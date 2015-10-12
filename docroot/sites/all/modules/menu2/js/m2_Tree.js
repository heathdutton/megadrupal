(function ($) {


  Drupal.behaviors.m2Prepare = {
    attach: function (context, settings) {
      $('.m2-tree', context).once('m2-tree', function () {
        var tree = $(this),
            tree_id = tree.attr('rel'),
            tree_item_all = $('li', tree),
            tree_item_parent_all = $('li:has(ul)', tree),
            cookie_id = tree_id + '-items-expanded';
        tree_item_parent_all.prepend($('<div class="ctrl-expand"></div>'));
        tree_item_all.prepend($('<div class="icon"></div>'));
     /* restore expanded states */
        var expanded_items = $.cookie(cookie_id) ? $.cookie(cookie_id).split('.') : [];
        $(expanded_items).each(function(){
          $('[rel=' + this + ']', tree).addClass('is-expanded');
        });
        $('li.active-trail', tree).addClass('is-expanded');
        $('li.active', tree).addClass('is-expanded');
     /* expand core */
        $('.ctrl-expand', tree).mousedown(function(){
          var tree_item = $(this).parent(),
              tree_item_id = tree_item.attr('rel'),
              expanded_items = $.cookie(cookie_id) ? $.cookie(cookie_id).split('.') : [];
          tree_item.toggleClass('is-expanded');
          if (tree_item.is('.is-expanded')) expanded_items.item_insert_unique(tree_item_id);
          else expanded_items.item_delete(tree_item_id);
          $.cookie(cookie_id, expanded_items.join('.'), {path: '/'});
        }).disableSelection();
      });
    }
  };


})(jQuery);