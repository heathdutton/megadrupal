// $Id$

$(document).ready(function()
{
	var contactMail = $("#restrict_ip_contact_mail").text().replace("[at]", "@");
	$("#restrict_ip_contact_mail").html("<a href=\"mailto:" + contactMail + ">" + contactMail + "</a>");
});