(function($){
  $(document).ready(function(){
    var menu_element = Drupal.settings.single_page.menu_element;
    $(menu_element+" a").click(function(event){
      $(".colorbox").colorbox();
    });
  });
})(jQuery);