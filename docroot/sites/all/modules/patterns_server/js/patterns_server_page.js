/**
 * @file 
 * Patterns_server main js.
 *
 * The patterns server's main page.
 */
jQuery(document).ready(function() {
    
    //when click on download link, download times grown by 1.
    var download_link = jQuery(".pattern_row .download-link");
    download_link.click(function() {
        var download_times = jQuery(this).parent().prev().prev();
        var numb = Number(download_times.text()) + 1; 
        download_times.children().text(numb);
    });

    //use moment.js to format upload time.
    jQuery(".pattern_row .upload-time").text(function(){
        return moment.unix(jQuery(this).attr("value")).fromNow();
    });

    //ajax
    jQuery(".info-link").click(function(){
        var url_div = jQuery(this).attr("href");
        url_div += " #one_pattern_div";
        jQuery("#one_pattern_div").load(url_div); 
        return false;
    });

    //search box
	jQuery('.span9 input#edit-search').addClass('span7');
	jQuery('.span12 input#edit-search').addClass('span10');

	//server or detail link
	var server_detail = jQuery('#all_patterns_div').attr('id');
	if (server_detail!='all_patterns_div') {
		var sd = jQuery('#one_pattern_div').find('.server_detail');
		sd.text('Home');
		sd.attr('href', sd.attr('name'));
	}
	

});
