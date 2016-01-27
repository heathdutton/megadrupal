

//adjusts the admin page so it looks correct
$(document).ready(function(){
	$("#right-toggle").click( function() {
		$("#right-toggle").hide();
		$("#content").width("953px");
		$("#sidebar-left").css("padding-left","775px");
		$("#right").css("float","none");
		$("#content").css("border-right","1px solid #1A4864");
		$("#content").css("border-left","1px solid #1A4864");
		$("#content").css("border-bottom","1px solid #1A4864");
		$("#content").css("margin","0");
	});
});
