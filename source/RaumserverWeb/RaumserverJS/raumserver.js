
var G_VERSION = "1.0.0";
//var G_RS_HOST = "http://10.0.0.47";
var G_RS_HOST = "http://10.0.0.2";
var G_RS_PORT = "8080";
var G_REQUEST = G_RS_HOST + ":" + G_RS_PORT + "/raumserver/controller/";
var G_DATAREQUEST = G_RS_HOST + ":" + G_RS_PORT + "/raumserver/data/";

var raumserver 


(function ($) { //an IIFE so safely alias jQuery to $
    $.Raumserver = function () { 
        this.xhrPool = [];
        this.raumserverOnline = false;
        this.raumserverVersion = "";
        this.raumkernelVersion = "";
        this.sessionId = this.generateGUID();              
    };


    $.Raumserver.prototype = {
    
        init: function () 
        {  
            this.logDebug('Initializing RaumserverJS ' + G_VERSION + ' on host: ' + G_RS_HOST + ':' + G_RS_PORT);  
            this.setupAjaxRequestPool();
            // we have to start a polling to check whether the raumserver is online or not, we do this by using the get version request
            this.setupOnlineCheckerRequest();                            
        },
               
        close: function () 
        {  
            this.logDebug("Closing RaumserverJS");  
            this.abortAjaxRequestPool();   
            this.abortSession();
        },
        
        
        abortSession: function () 
        {             
            this.logDebug("Aborting session: " + this.sessionId );                          
            $.ajax({
                url: G_REQUEST + "killSession?sessionId=" + this.sessionId,                
                cache: false,
                async: false ,            
                success: function(res, status, xhr) 
                {		
                    this.logDebug("Aborting session successfully stacked: " + this.sessionId );   
                },
                error: function(xhr, textStatus, errorThrown)
                {                                                                    
                    this.logDebug("Error when aborting session: " + this.sessionId ); 	                 				                    
                }
            });	
        },
        
        
        generateGUID : function() 
        {
            return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
                  var r = Math.random() * 16 | 0,
                    v = c == 'x' ? r : (r & 0x3 | 0x8);
                  return v.toString(16);
              }).toUpperCase();
        },
        
        
        setRaumserverOnline: function(isOnline) 
        {
            if( this.raumserverOnline != isOnline)
            {
                raumserver.raumserverOnline = isOnline;   
                if (raumserver.raumserverOnline)
                {
                    raumserver.logDebug("Raumserver is now online"); 
                    // restart the polling requests due they were stopped when system was offline
                    setTimeout(function(){ raumserver.setupZoneConfigPollingRequest(""); }, 100); 
                    setTimeout(function(){ raumserver.setupGetRendererStatePollingRequest(""); }, 100); 
                    setTimeout(function(){ raumserver.setupZoneMediaListPollingRequest(""); }, 100);                                         
                }
                else 
                    raumserver.logDebug("Raumserver is now offline");
                // raise event
                $.event.trigger({type: "RS.OnlineStateChanged", online: raumserver.raumserverOnline, versionServer: raumserver.raumserverVersion, versionKernel: raumserver.raumkernelVersion});
            }
        },
        
        // Ajax stuff ////////////////////////// 
        setupAjaxRequestPool: function() 
        {
            this.logDebug("Setting up ajax request pool");  
            $(document).ajaxSend(function(e, jqXHR, options) { 
                //raumserver.logDebug("Pushing ajax request to pool");
                raumserver.xhrPool.push(jqXHR); } 
            );	  	  
            $(document).ajaxComplete(function(e, jqXHR, options) { 
                //raumserver.logDebug("Removing ajax request from pool");
                raumserver.xhrPool = $.grep(raumserver.xhrPool, function(x){return x!=jqXHR}); }
            );                       
        },
        
        abortAjaxRequestPool: function()
        {
            this.logDebug("Aborting all ajax requests");  
            $.each(this.xhrPool, function(idx, jqXHR) { jqXHR.abort();});
        },
        
        
        // Polling stuff //////////////////////
        setupOnlineCheckerRequest: function()
        {
            this.logDebug("Do request for checking system availability");  
            $.ajax({
                url: G_DATAREQUEST + "getVersion",
                cache: false,             
                success: function(res, status, xhr) 
                {		
                    try
                    {                          
                        versionObject = $.parseJSON(res);
                        raumserver.raumserverVersion = versionObject.raumserverLib;
                        raumserver.raumkernelVersion = versionObject.raumkernelLib;
                        raumserver.setRaumserverOnline(true);
                    }
                    catch (exception)
                    {
                        raumserver.logException(exception);
                    }
                    setTimeout(function(){raumserver.setupOnlineCheckerRequest(); }, 5000);
                },
                error: function(xhr, textStatus, errorThrown)
                {                    
                    raumserver.raumserverVersion = "";
                    raumserver.raumkernelVersion = "";
                    raumserver.setRaumserverOnline(false);                
                    setTimeout(function(){raumserver.setupOnlineCheckerRequest(); }, 5000);	                 				                    
                }
            });	
        },
        
        
        setupZoneConfigPollingRequest: function(_updateId)
        {         
            // when the raumserver is not online, then skip the request. 
            // It will be triggered again when the raumfeld system gets online
            if (!raumserver.raumserverOnline)
                return;
        
            this.logDebug("Do zone polling request with update id: " + _updateId);                          
            $.ajax({
                url: G_DATAREQUEST + "getZoneConfig?updateId=" + _updateId + "&sessionId="+this.sessionId,                
                cache: false,
                beforeSend: function (request)
                {
                    //request.setRequestHeader("updateID", _updateId);
                    request.setRequestHeader("sessionId", this.sessionId);
                },
                success: function(res, status, xhr) 
                {		
                    try
                    {       
                        raumserver.logDebug("Zone configuration changed");                        
                        zoneConfigObject = $.parseJSON(res);                            
                        $.event.trigger({type: "RS.ZoneConfigurationChanged", object: zoneConfigObject});                        
                        setTimeout(function(){ raumserver.setupZoneConfigPollingRequest(xhr.getResponseHeader("updateId")); }, 1);                        
                    }
                    catch (exception)
                    {
                        raumserver.logException(exception);
                        setTimeout(function(){raumserver.setupZoneConfigPollingRequest(""); }, 5000);	
                    }                    
                },
                error: function(xhr, textStatus, errorThrown)
                {                                                                    
                    setTimeout(function(){raumserver.setupZoneConfigPollingRequest(""); }, 3000);	                 				                    
                }
            });	
        },
        
                
        setupGetRendererStatePollingRequest: function(_updateId)
        {         
            // when the raumserver is not online, then skip the request. 
            // It will be triggered again when the raumfeld system gets online
            if (!raumserver.raumserverOnline)
                return;
        
            this.logDebug("Do renderer state polling request with update id: " + _updateId);                          
            $.ajax({
                url: G_DATAREQUEST + "getRendererState?listAll=true&updateId=" + _updateId + "&sessionId="+this.sessionId,                
                cache: false,
                beforeSend: function (request)
                {                    
                    request.setRequestHeader("sessionId", this.sessionId);
                },
                success: function(res, status, xhr) 
                {		
                    try
                    {       
                        raumserver.logDebug("Renderer state changed");                        
                        rendererStateObject = $.parseJSON(res);                                       
                        $.event.trigger({type: "RS.RendererStateChanged", object: rendererStateObject});                        
                        setTimeout(function(){ raumserver.setupGetRendererStatePollingRequest(xhr.getResponseHeader("updateId")); }, 1);                        
                    }
                    catch (exception)
                    {
                        raumserver.logException(exception);
                        setTimeout(function(){raumserver.setupGetRendererStatePollingRequest(""); }, 5000);	
                    }                    
                },
                error: function(xhr, textStatus, errorThrown)
                {                                                                    
                    setTimeout(function(){raumserver.setupGetRendererStatePollingRequest(""); }, 3000);	                 				                    
                }
            });	
        },
        
        
        setupZoneMediaListPollingRequest: function(_updateId)
        {         
            // when the raumserver is not online, then skip the request. 
            // It will be triggered again when the raumfeld system gets online
            if (!raumserver.raumserverOnline)
                return;
        
            this.logDebug("Do zone media list polling request with update id: " + _updateId);                          
            $.ajax({
                url: G_DATAREQUEST + "getZoneMediaList?updateId="+_updateId  + "&sessionId="+this.sessionId,                
                cache: false,
                beforeSend: function (request)
                {                    
                    request.setRequestHeader("sessionId", this.sessionId);
                },
                success: function(res, status, xhr) 
                {		
                    try
                    {       
                        raumserver.logDebug("Media list on zone changed");                        
                        zoneMediaListObject = $.parseJSON(res);                                
                        $.event.trigger({type: "RS.ZoneMediaListChanged", object: zoneMediaListObject});
                        setTimeout(function(){ raumserver.setupZoneMediaListPollingRequest(xhr.getResponseHeader("updateId")); }, 1);                        
                    }
                    catch (exception)
                    {
                        raumserver.logException(exception);
                        setTimeout(function(){raumserver.setupZoneMediaListPollingRequest(""); }, 5000);	
                    }                    
                },
                error: function(xhr, textStatus, errorThrown)
                {                                                                    
                    setTimeout(function(){raumserver.setupZoneMediaListPollingRequest(""); }, 3000);	                 				                    
                }
            });	
        },
	  
      
        // Log stuff ////////////////////////// 
        logDebug: function (logText) 
        {      
            console.log(this.getReadableDate() + " RaumserverJS: " + logText);  
            this.logToViewer(0, logText, "");
        },
        
        
        logInfo: function (logText) 
        { 
            console.log(this.getReadableDate() + " RaumserverJS: " + logText);    
            this.logToViewer(1,logText, "");            
        },
        
        logWarning: function (logText) 
        { 
            console.log(this.getReadableDate() + " RaumserverJS: " + logText);    
            this.logToViewer(2,logText, "");            
        },
        
        logError: function (logText) 
        { 
            console.log(this.getReadableDate() + " RaumserverJS: " + logText);    
            this.logToViewer(3,logText, "");            
        },
        
        logException: function (exception) 
        {         
            console.log(this.getReadableDate() + "RaumserverJS: " + "EXCEPTION: " + exception.message + "\n\n" + exception.stack);    
            this.logToViewer(4,exception.message,exception.stack);            
        },
        
        logToViewer: function (logType, logText, logStack) 
        {
            $.event.trigger({type: "RS.Log", logtype: logType, logText: logText, logStack: logStack, logTime: this.getReadableDate()});                 
        },
        
        getReadableDate: function()
        {
            var newDate = new Date();            
            dateString = newDate.toUTCString();
            return dateString;
        }
    };
 

}(jQuery));


/*
$( document ).ready(function() {
    raumserver = new $.Raumserver();
    raumserver.init();
});

window.onbeforeunload = function() 
{  
    raumserver.close();
}
*/
            



