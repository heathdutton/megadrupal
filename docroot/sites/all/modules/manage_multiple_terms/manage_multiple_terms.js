
(function ($) {

Drupal.manageMultipleTerms = Drupal.manageMultipleTerms || {};

Drupal.behaviors.manageMultipleTerms = {
  attach: function (context, settings) {
    $('#collapse-expand-all').click(function() {
      if($(this).hasClass('collapsed-all')) {
        $('.manage-multiple-terms-table').each(function() {
          $(this).children('tbody').show();
          $(this).removeClass('collapsed').addClass("expanded");
          $(this).find('a.collapse-expand-button').html(Drupal.t('Collapse'));
        });
        $(this).removeClass('collapsed-all').addClass('expanded-all');
        $(this).html(Drupal.t("Collapse All"));
      }
      else {
        $('.manage-multiple-terms-table').each(function() {
          $(this).children('tbody').hide();
          $(this).removeClass('expanded').addClass("collapsed");
          $(this).find('a.collapse-expand-button').html(Drupal.t('Expand'));
        });
        $(this).removeClass('expanded-all').addClass('collapsed-all');
        $(this).html(Drupal.t("Expand All"));        
      }
      return false; 
    });
    $('.collapse-expand-button').click(function() {
      var self = $(this);
      var tbody = $(this).closest('.manage-multiple-terms-table').children('tbody');
      if(tbody.closest('table').hasClass('collapsed')) {
        tbody.show();
        $(this).closest('table').removeClass('collapsed').addClass('expanded');
        self.html('Collapse');
        if($('#collapse-expand-all').hasClass('collapsed-all')) {
          var counter = 0;
          $('.manage-multiple-terms-table').each(function() {
            if($(this).hasClass('collapsed')) {
              counter ++;
            }
          });
          if(counter === 0) {
            $('#collapse-expand-all').removeClass('collapsed-all').addClass('expanded-all');
            $('#collapse-expand-all').html(Drupal.t("Collapse All"));
          }
        }
      }
      else {
        tbody.hide();
        $(this).closest('table').removeClass('expanded').addClass('collapsed');
        self.html('Expand');
        if($('#collapse-expand-all').hasClass('expanded-all')) {
          var counter = 0;
          $('.manage-multiple-terms-table').each(function() {
            if($(this).hasClass('expanded')) {
              counter ++;
            }
          });
          if(counter === 0) {
            $('#collapse-expand-all').removeClass('expanded-all').addClass('collapsed-all');
            $('#collapse-expand-all').html(Drupal.t("Expand All"));
          }
        }
      }
      return false;
    });
    $('.add-term-button').click(function() {
      var closest_table = $(this).closest('table');
      var new_tr = closest_table.find('.term-row-template').clone();
      new_tr.removeClass('term-row-template');
      new_tr.addClass('draggable').addClass('vocabulary-items-table');
      new_tr.show();
      new_tr.find('.delete-term-button').click(function() {
        var tr = $(this).closest('tr').remove();
        return false;
      });      
      closest_table.append(new_tr);
      var table = $(this).closest('table.manage-multiple-terms-table');
      if(table.hasClass('collapsed')) {
        table.children('tbody').show();
        table.removeClass('collapsed').addClass('expanded');
        table.find('a.collapse-expand-button').html('Collapse');
        if($('#collapse-expand-all').hasClass('collapsed-all')) {
          var counter = 0;
          $('.manage-multiple-terms-table').each(function() {
            if($(this).hasClass('collapsed')) {
              counter ++;
            }
          });
          if(counter === 0) {
            $('#collapse-expand-all').removeClass('collapsed-all').addClass('expanded-all');
            $('#collapse-expand-all').html(Drupal.t("Collapse All"));
          }
        }        
      }
      return false;
    });
    $('.delete-term-button').click(function() {
      var tr = $(this).closest('tr');
      var term_id = tr.find('input.term-id');
      var term_parent = tr.find('input.term-parent').val();
      var tid = term_id.val();
      tr.hide();
      tr.find('input.deletable-field').val('');
      var children = tr.closest('.manage-multiple-terms-table').find('input.term-parent[value="' + tid + '"]');
      var new_children = [];
      children.each(function() {
        $(this).val(term_parent);
        $(this).closest('td').find('.indentation:first-child').remove();
        $(this).closest('td').find('input.term-depth').each(function() {
          $(this).val($(this).val() - 1); 
        });
        var _tid = $(this).closest('td').find('input.term-id').val();
        var ch = tr.closest('.manage-multiple-terms-table').find('input.term-parent[value="' + _tid + '"]');
        ch.each(function() {
          new_children.push($(this));
        });
      });
      children = new_children;
      new_children = [];
      while(children.length) {
        $(children).each(function() {
          $(this).closest('td').find('.indentation:first-child').remove();
          $(this).closest('td').find('input.term-depth').each(function() {
            $(this).val($(this).val() - 1); 
          });
          var _tid = $(this).closest('td').find('input.term-id').val();
          var ch = tr.closest('.manage-multiple-terms-table').find('input.term-parent[value="' + _tid + '"]');
          ch.each(function() {
            new_children.push($(this));
          });
        });
        children = new_children;
        new_children = [];          
      };
      return false;
    });
  }
};
})(jQuery);
