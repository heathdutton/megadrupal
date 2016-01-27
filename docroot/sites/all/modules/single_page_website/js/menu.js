(function($){
  $(document).ready(function(){
    var menu_element = Drupal.settings.single_page.menu_element;
    var basePath = Drupal.settings.basePath;
    var pathPrefix = Drupal.settings.pathPrefix;
    $(menu_element+" a").each(function(index) {
	  var hr = this.href.split(basePath);
	  var anchor = hr[hr.length-1]
      this.href = basePath + pathPrefix + "#" + "spw-" + anchor.replace(/\//g,"-");
    });
  });
})(jQuery);
