
var G_VERSION = "1.0.0";
var G_RS_HOST = "http://10.0.0.47";
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
            // 
            this.setupZoneConfigPollingRequest("");
        },
               
        close: function () 
        {  
            this.logDebug("Closing RaumserverJS");  
            this.abortAjaxRequestPool();            
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
                    raumserver.logDebug("Raumserver is now online"); 
                else 
                    raumserver.logDebug("Raumserver is now offline");
                // raise event
                $.event.trigger({type: "RS.OnlineStateChanged", online: raumserver.raumserverOnline, versionServer: raumserver.raumserverVersion ,versionKernel: raumserver.raumkernelVersion});
            }
        },
        
        // Ajax stuff ////////////////////////// 
        setupAjaxRequestPool: function() 
        {
            this.logDebug("Setting up ajax request pool");  
            $(document).ajaxSend(function(e, jqXHR, options) { 
                raumserver.logDebug("Pushing ajax request to pool");
                raumserver.xhrPool.push(jqXHR); } 
            );	  	  
            $(document).ajaxComplete(function(e, jqXHR, options) { 
                raumserver.logDebug("Removing ajax request from pool");
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
            this.logDebug("Initializing server online checker request");  
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
            this.logDebug("Initializing zone polling request with update id: " + this.updateIdZoneConfigPolling);  
            $.ajax({
                url: G_DATAREQUEST + "getZoneConfig",
                cache: false,
                beforeSend: function (request)
                {
                    request.setRequestHeader("updateID", _updateId);
                    request.setRequestHeader("sessionID", this.sessionId);
                },
                success: function(res, status, xhr) 
                {		
                    try
                    {       
                        this.logDebug("Zone config changed");                        
                        zoneConfigObject = $.parseJSON(res);
                        // TODO: @@@
                        raumserver.setupZonePollingRequest(xhr.getResponseHeader("updateID"))
                    }
                    catch (exception)
                    {
                        raumserver.logException(exception);
                    }
                    raumserver.setupZoneConfigPollingRequest("");
                },
                error: function(xhr, textStatus, errorThrown)
                {                                                                    
                    setTimeout(function(){raumserver.setupZoneConfigPollingRequest(""); }, 3000);	                 				                    
                }
            });	
        },
	  
      
        // Log stuff ////////////////////////// 
        logDebug: function (logText) 
        {      
            console.log("RaumserverJS: " + logText);  
            this.logToViewer(logText);
        },
        
        logError: function (logText) 
        { 
            console.log("RaumserverJS: " + logText);    
            this.logToViewer(logText);            
        },
        
        logException: function (exception) 
        { 
            console.log("RaumserverJS: " + "EXCEPTION: " + exception.message + "\n\n" + exception.stack);    
            this.logToViewer("EXCEPTION: " + exception.message + "\n\n" + exception.stack);            
        },
        
        logToViewer: function (logText) 
        {
            var logViewerDOM = $('#logViewerContainer')[0];            
            logViewerDOM.appendChild(document.createTextNode(logText));
            logViewerDOM.appendChild(document.createElement('br'));            
            logViewerDOM.scrollTop = logViewerDOM.scrollHeight;
        }
    };
 

}(jQuery));


$( document ).ready(function() {
    raumserver = new $.Raumserver();
    raumserver.init();
});

window.onbeforeunload = function() 
{  
    raumserver.close();
}
            



