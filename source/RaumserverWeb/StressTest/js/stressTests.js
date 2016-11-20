
var compiledTemplates = [];

$(document).on("RS.OnlineStateChanged", onlineStateChanged);
$(document).on("RS.ZoneMediaListChanged", zoneMediaListChanged);
$(document).on("RS.RendererStateChanged", rendererStateChanged);
$(document).on("RS.ZoneConfigurationChanged", zoneConfigurationStateChanged);
$(document).on("RS.Log", log);


$( document ).ready(function() {
    compileTemplates();
    raumserver = new $.Raumserver("http://10.0.0.2:8080");
    raumserver.init();
});


window.onbeforeunload = function() 
{  
    raumserver.close();
}


function compileTemplates()
{    
    compiledTemplates['rendererInfoTemplate'] = Handlebars.compile($("#rendererInfoTemplate").html());   
    compiledTemplates['mediaListTemplate'] = Handlebars.compile($("#mediaListTemplate").html());   
    compiledTemplates['logItemTemplate'] = Handlebars.compile($("#logItemTemplate").html());  
    compiledTemplates['systemStatusTemplate'] = Handlebars.compile($("#systemStatusTemplate").html());  
    compiledTemplates['zoneConfigInfoTemplate'] = Handlebars.compile($("#zoneConfigInfoTemplate").html()); 
};


function log(e)
{   
    var logViewerDOM = $('#logViewerContainer');                       
    logViewerDOM.append(compiledTemplates['logItemTemplate'](e));
    logViewerDOM = $('#logViewerContainer')[0]; 
    logViewerDOM.scrollTop = logViewerDOM.scrollHeight; 
};


function onlineStateChanged(e) 
{ 
    var systemStatusDOM = $('#systemStatusContainer');                       
    systemStatusDOM.html(compiledTemplates['systemStatusTemplate'](e));
}


function zoneConfigurationStateChanged(e) 
{     
    var zoneConfiguration = e.object    
    
    $('#zoneConfigContainer').html("");
    for (var i = 0; i < zoneConfiguration.length; i++)
    {  
        if(zoneConfiguration[i].rooms.length > 0)
        {
            if(!zoneConfiguration[i].name)
                zoneConfiguration[i].name = "Unassigned rooms";
            zoneConfigInfo = compiledTemplates['zoneConfigInfoTemplate'](zoneConfiguration[i]);        
            $('#zoneConfigContainer').append(zoneConfigInfo);          
        }
    }
}


function rendererStateChanged(e) 
{ 
    var rendererStateObject = e.object;
    var drawIndex = 0;
    for (var i = 0; i < rendererStateObject.length; i++)
    {
        if (rendererStateObject[i].isZoneRenderer)
        {
            drawIndex++;
            var rendererInfo;                                   
            rendererInfo = compiledTemplates['rendererInfoTemplate'](rendererStateObject[i]);      
            //raumserver.logDebug(rendererStateObject[i].mediaItem.albumArtUri);
             $('#zoneInfoContainer'+(drawIndex).toString()).html(rendererInfo);
            // TODO: Store uuid of zone for playlists to find the right place
            // redraw playlists?!?!?   

            
        }            
    }    
    // clear non used ones
    for (var c = (drawIndex+1); c < 4; c++)
    {
        $('#zoneInfoContainer1'+(c).toString()).html("");
    }
    
}


function zoneMediaListChanged(e) 
{ 
    var zoneMediaListObject = e.object;
    var mediaListItems;  
    for (var i = 0; i < zoneMediaListObject.length; i++)
    {
        mediaListItems = "";
        // we get here each zone
        for (var r = 0; r < zoneMediaListObject[i].items.length; r++)
        {
            mediaListItems += compiledTemplates['mediaListTemplate'](zoneMediaListObject[i].items[r]);                    
        }
        // TODO find correct index!!!
        $('#zoneMediaListContainer'+(i+1).toString()).html(mediaListItems);
        
        //for (var c = (i+1); c < 4; c++)
        //{
        //    $('#zoneMediaList'+(c).toString()).html("");
        //}
    }    
}



