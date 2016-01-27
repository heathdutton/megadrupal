(function ($) {


  Drupal.behaviors.m2TreeSelectionPrepare = {
    attach: function (context, settings) {
      $('body', context).once('m2-body', function () {
        var tab_manage = $(this);
        $('*:not(input)', tab_manage).disableSelection();
      });
    }
  };


  Drupal.behaviors.m2TreeDragAndDropCore = {
    attach: function (context, settings) {
      $('.m2-items-manage', context).once('m2-items-manage', function () {
        var tree = $(this);
     /* prepare func */
        function m2_tree_items_class_prepare(tree){
          $('li.first', tree).removeClass('first');
          $('li.last', tree).removeClass('last');
          $('li.single', tree).removeClass('single');
          $('li.has-children', tree).removeClass('has-children');
          $('li:first-child', tree).addClass('first');
          $('li:last-child', tree).addClass('last');
          $('li:has(li)', tree).addClass('has-children');
        }
     /* prepare environment */
        $('li:not(:has(ul))', tree).append('<ul></ul>');
        $('<div class="m2-drop-place-0"></div>').insertBefore($('.icon', tree));
        $('<div class="m2-drop-place-1"></div>').insertBefore($('.icon', tree));
        m2_tree_items_class_prepare(tree);
     /* drag and drop process */
        $('.icon', tree).m2Drag();
        $('.m2-drop-place-0, .m2-drop-place-1', tree).m2Drop({
          drag_parent_el: 'li',
          on_drop: function(drag_el, drop_el){
            var drag_li = drag_el.parent().parent(),
                drop_li = drop_el.parent().parent(),
                drop_li_ul = $('> ul', drop_li);
            if (drop_li_ul.get(0).nodeName == 'UL' &&
                   drag_li.get(0).nodeName == 'LI' &&
                   drop_li.get(0).nodeName == 'LI' && drag_li.get(0) != drop_li.get(0) && !$.contains(drag_li.get(0), drop_li.get(0))) {
              if (drop_el.is('.m2-drop-place-0')) drag_li.insertBefore(drop_li);
              if (drop_el.is('.m2-drop-place-1')) drop_li_ul.append(drag_li);
              m2_tree_items_class_prepare(tree);
              tree.trigger('change');
            }
        }});
      });
    }
  };


  Drupal.behaviors.m2FormTreeSave = {
    attach: function (context, settings) {
      $('.m2-items-manage-form', context).once('m2-items-manage-form', function () {
        var form = $(this),
            tree = $('.m2-items-manage', form),
            return_tree_state = $('#edit-return-tree-state', form),
            button_save = $('#edit-button-save-tree-changes', form);
        if (tree.length == 1) {
          tree.change(function(event){
            button_save.show();
            tree.change = null;
          });
          button_save.click(function(){
            var item_states = [],
                item_weight = 1;
            $('.item-value', form).each(function(){
              item_states.push($(this).attr('rel') + ',' + (item_weight++) + ',' + ($(this).parents('ul').length - 1));
            });
            return_tree_state.attr('value', item_states.join('|'));
          });
        }
      });
    }
  };


})(jQuery);