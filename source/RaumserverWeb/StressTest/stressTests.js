

$(document).on("RS.OnlineStateChanged", onlineStateChanged);


function onlineStateChanged(e) 
{ 
	$('#rsStatus').html(e.online.toString());
    $('#rsVersion').html(e.versionServer.toString());
    $('#rkVersion').html(e.versionKernel.toString());
}