
var compiledTemplates = [];

$(document).on("RS.OnlineStateChanged", onlineStateChanged);
$(document).on("RS.ZoneMediaListChanged", zoneMediaListChanged);
$(document).on("RS.RendererStateChanged", rendererStateChanged);
$(document).on("RS.ZoneConfigurationChanged", zoneConfigurationStateChanged);
$(document).on("RS.Log", log);


$( document ).ready(function() {
    compileTemplates();
    raumserver = new $.Raumserver();
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
    
    $('#zoneConfig').html("");
    for (var i = 0; i < zoneConfiguration.length; i++)
    {
        // rooms in zones
        var rooms = "";
        for (var r = 0; r < zoneConfiguration[i].rooms.length; r++)
        {
            if (rooms) 
                rooms += ", ";
            rooms += zoneConfiguration[i].rooms[r].name;
        }
    
        if(!zoneConfiguration[i].UDN)
            $('#zoneConfig').append("<div>Not assigned: ("+rooms+")</div>");
        else
            $('#zoneConfig').append("<div>Zone: " + zoneConfiguration[i].name + " ("+rooms+")</div>");
    }
    
    // TODO: renderer state changed should be called by kernel?!
}

function rendererStateChanged(e) 
{ 
    var rendererStateObject = e.object;
    for (var i = 0; i < rendererStateObject.length; i++)
    {
        var rendererInfo;                   
        rendererInfo = compiledTemplates['rendererInfoTemplate'](rendererStateObject[i]);        
         $('#zone'+(i+1).toString()).html(rendererInfo);
        // TODO: Store uuid of zone for playlists to find the right place
        // redraw playlists?!?!?        
    }    
    // clear non used ones
    for (var c = (i+1); c < 4; c++)
    {
        $('#zone'+(c).toString()).html("");
    }
    
}

function zoneMediaListChanged(e) 
{ 
    var zoneMediaListObject = e.object;
    for (var i = 0; i < zoneMediaListObject.length; i++)
    {
        var mediaListItems;    
        // we get here each zone
        for (var r = 0; r < zoneMediaListObject[i].items.length; r++)
        {
            mediaListItems += compiledTemplates['mediaListTemplate'](zoneMediaListObject[i].items[r]);                    
        }
        // TODO find correct index!!!
        $('#zoneMediaList'+(i+1).toString()).html(mediaListItems);
        
        //for (var c = (i+1); c < 4; c++)
        //{
        //    $('#zoneMediaList'+(c).toString()).html("");
        //}
    }    
}



