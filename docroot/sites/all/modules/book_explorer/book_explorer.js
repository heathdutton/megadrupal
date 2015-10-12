(function ($) {

Drupal.behaviors.book_explorer = {
  attach: function() {
    function selector_activate(control) {
      $(control.menu).show();
      $(control).removeClass('book-explorer-collapsed').addClass('book-explorer-expanded');
      control.expanded = true;
    }

    function selector_deactivate(control) {
      $('ul', control.list_item).hide();
      $('.book-explorer-toggle', control.list_item).each(function () { 
        this.expanded = false; 
      }).removeClass('book-explorer-expanded').addClass('book-explorer-collapsed');
      $(control).removeClass('book-explorer-expanded').addClass('book-explorer-collapsed');
      control.expanded = false;
    }
    
    $('.book-explorer').once().each(function () {
      $('.expanded', this).each(function () {
        var list_item = this;      
        $('<a href="#" class="book-explorer-toggle">&nbsp;</a>').insertBefore(this).each(function () {
          this.list_item = list_item;
          if ($('a.active', list_item).length) {
            this.menu = $('>ul', list_item);
            $(this).addClass('book-explorer-expanded');
            this.expanded = true;
          }
          else {
            this.menu = $('>ul', list_item).hide();
            $(this).addClass('book-explorer-collapsed');
            this.expanded = false;
          }
          $(this).click(function () {
            if (this.expanded) {
              selector_deactivate(this);
            }
            else {
              selector_activate(this);
            }
            return false;
          });
        });
      });
    });
  }
};

})(jQuery);
