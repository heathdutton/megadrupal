(function($){
  $(document).ready(function(){
    var basePath = Drupal.settings.basePath;
    var pathPrefix = Drupal.settings.pathPrefix;

    $(".navbar-nav a").each(function(index) {
      var hr = this.href.split(basePath);
      var anchor = hr[hr.length-1];

      //make active first menu item
      if (index == 0) {
        $(this).addClass("active");
        $(this).parent().addClass("active");
      }

      if ((anchor != 'user') && (anchor != 'logout')) {
        this.href = basePath + pathPrefix + "#" + anchor.replace(/\//g,"-");
        div_id_top = $("#" + anchor).offset().top;
      }
    });
  });
})(jQuery);
