/* jQuery */
jQuery.noConflict();
jQuery(document).ready(function(){

    jQuery('.cms-menu ul li').find("ul").hide();
    jQuery(".cms-menu ul li").hover(
      function() { jQuery(this).find("ul").show(); },
      function() { jQuery(this).find("ul").hide(); }
    );

});
