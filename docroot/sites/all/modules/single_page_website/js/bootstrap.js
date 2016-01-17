(function($){
  $(document).ready(function(){
    $('aside').remove();

    var header_element = Drupal.settings.single_page.header_element;
    var menu_element = Drupal.settings.single_page.menu_element;

    $('.single_page_wrapper').css({'padding-top': '0px'});
    $('.single_page').css('padding-top', '72px');
    $('.single_page:first').css('padding-top', '92px');

    margin_left_sp = $('#single_page_wrapper').offset().left;

    $(header_element).css({
      'position' : 'fixed',
      'margin-left': margin_left_sp,
      'margin-top': '20px',
      'width': '100%', 
      'z-index' : '500',
    }); 

    var box = $(header_element);
    var top = box.offset().top;

    //for scroll-sliding menu
    $(window).scroll(function(){

      var windowpos = $(window).scrollTop();
      if (windowpos == 0) {
        box.css({
          'margin-top': '20px',
        });     
      }
      else if (windowpos <= 20) {
        delta = 20 - windowpos;
        box.css({
          'margin-top': delta,
        });
      }
      else {
        margin_left_sp = $('.single_page_wrapper').offset().left;
        box.css ({
          'position' : 'fixed',
          'margin_left': margin_left_sp,
          'width': '100%', 
          'margin-top' : '0px',
        });
      }; 
      
      //for activate menu item at scrolling page      
      var scrollItems = $('.navbar-nav').find("a").map(function() {
        var item = $(this).attr("href").split("/");
        offset = $(item[1]).offset().top;
        if (offset < (windowpos + 45)) { 
          $(menu_element + " li").removeClass("active");
          $(menu_element + " li a").removeClass("active");
          $(this).addClass("active");
          $(this).parent().addClass("active");
          return item[1]; 
        }
      });

    });

    //for change menu position, width at resizing window 
    $(window).resize(function() { 
      margin_left_sp = $('.single_page_wrapper').offset().left;
      $(header_element).css({
        'margin-left': margin_left_sp,
        'width': '100%', 
      });
    });

    $('.main-container > .row > aside').remove();
    $('.col-sm-9 > .tabs--primary').filter( ':first' ).remove();
    $('.breadcrumb').remove(); 
  });
})(jQuery);