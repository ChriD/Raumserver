

$(document).on("RS.OnlineStateChanged", onlineStateChanged);


function onlineStateChanged(e) 
{
    console.log("............");  
	$('#rsStatus').html(e.online.toString());
    $('#rsVersion').html(e.versionServer.toString());
    $('#rkVersion').html(e.versionKernel.toString());
}