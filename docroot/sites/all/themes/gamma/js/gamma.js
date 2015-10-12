// $Id: gamma.js,v 1.1 2011/01/07 16:10:59 himerus Exp $.js,v 1.1.2.3 2010/09/11 15:23:25 himerus Exp $

(function ($) {
  /**
   * @todo   Must fix the interaction between active menu items
   * @todo   Change the order of the hover out fade back to normal, and sub-menu hiding
   *         This should be set to close the menu fully (after short delay) then fade the menu color back to normal
   * @todo  Make sure the menus accomodate menus that are only 2 wide on the top level.
   */
  $(document).ready(function(){
    /**
     * Respect primary menu trails, regardless of menu structure
     */
    var currentURL = document.location.pathname;
    var activeTopLevelPageArray = currentURL.split('/');
    var activeTopLevelPage = activeTopLevelPageArray[1];
   
    // applying the class to what we find
    $('#region-menu ul li a[href^=/'+activeTopLevelPage+']')
      .addClass('active');
    // now making the nav menu do what we want with styles for the active element
    $('#region-menu ul li a.active:first')
      .addClass('current-main') // assign main active bg to active link
      .parent('li') // select the parent list item
      .addClass('active-trail'); // then assign a class to it for manipulation
   
    /* If there is no match, just activate the "home" menu item/tab */
    var isActive = $('#region-menu li.active-trail').size();
    if (isActive < 1) {
      $('#region-menu li:first').addClass('active-trail');
    }
  
    // make the top level menu item remain in a hover state while on a submenu in the dropdown
    $('#region-menu ul.main-menu > li ul').hover(function(){
    // hover in
    $(this).parent('li').addClass('hover');
    }, function(){
    // hover out
    $(this).parent('li').removeClass('hover');
    });
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    // make the menu sexy
    $('#region-menu div.main-menu > ul.main-menu > li').hover(
      // hover in
      function(){
          var menu = $('#region-menu div.main-menu > ul.main-menu > li');
          // find the position of the parent LI in the menu
          var parentIndex = menu.index(this);
          
          // find the number of total items in the menu
          var totalNum = menu.size();
          // each dropdown is the width of the next 3 menu items by default
          // if the menu item is one of the last two, we need to change the way
          // the width and positioning is calculated.
          if(totalNum - parentIndex < 3) {
            var compare1 = totalNum - 1;
            var compare2 = totalNum - 2;
            var compare3 = totalNum - 3;
            var offset = $(this).offset();
            var offsetReal = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare3+')').offset();
            leftOffset = offsetReal.left - offset.left - 1;
            
            
            var item1 = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare1+')').outerWidth();
            var item2 = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare2+')').outerWidth();
            var item3 = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare3+')').outerWidth();
            var menuWidth = item1 + item2 + item3 + 2;
          }
          else {
            // add up this and the next two menu items to get the appropriate width
            var compare1 = parentIndex;
            var compare2 = parentIndex + 1;
            var compare3 = parentIndex + 2;
            // move the menu over just a tad to line up the outer border
            leftOffset = -1;
            
            var item1 = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare1+')').outerWidth();
            var item2 = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare2+')').outerWidth();
            var item3 = $('#region-menu div.main-menu > ul.main-menu > li:eq('+compare3+')').outerWidth();
            var menuWidth = item1 + item2 + item3 + 3;
          }
          
          
          $(this)
            //.addClass('isPositioned')
            .children('ul:eq(0)')
            .css('width', menuWidth)
            .css('left', leftOffset)
            //.slideDown('normal')
            ;
          //$(this).children('ul').css('width', menuWidth);
          $(this).find('ul').css('width', menuWidth);
      }, 
      // hover out
      function(){
         
    });
    
    
    
    
    
    
    
  });
})(jQuery);