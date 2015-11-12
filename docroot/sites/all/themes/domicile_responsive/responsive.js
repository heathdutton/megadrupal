// once the document has finished loading
jQuery(document).ready(function()
  {

  function smallScreen () {
    if (window.innerWidth < 600) {
      jQuery(".region-nav-left").appendTo(jQuery('#content'));
      jQuery("#content").css("margin-top", "3em");
    } else {
      jQuery(".region-nav-left").appendTo(jQuery('#nav'));
    }
  }
  smallScreen(); 
  jQuery(window).resize(smallScreen);
  
  });
