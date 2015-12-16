(function ($) {

Drupal.cloudwords = Drupal.cloudwords || {};

Drupal.cloudwords.updateChecked = function(view, op, ids) {
  if (ids) {
    $('.vbo-table-this-page strong').text(ids.length + ' rows');
    var string = ids.join(',');
    var token = Drupal.settings.cloudwords.token;
    $.get(Drupal.settings.cloudwords.ajaxUrl + '/' + view + '/' + op + '/' + string + '?token=' + token, function(data) {
      $('.cloudwords-item-count').text(data);
    });
  }
}

Drupal.behaviors.cloudwords = {
  attach: function (context, settings) {

    // $('.vbo-table-select-all-pages').once(function() {
    //   $(this).click(function(event) {
    //     event.preventDefault();
    //     $.get('/system/cloudwords/ajax/' + view + '/' 'all', function(data) {
    //       $('.cloudwords-item-count').text(data);
    //     });
    //   });
    // });

    $.each(['block_1', 'block_2'], function(i, display) {
      $('.view-display-id-' + display + ' .vbo-table-select-all').once('cloudwords', function() {
        $(this).change(function() {
          var op = 'remove';
          if ($(this).attr('checked')) {
            op = 'add';
          }
          var output = [];
          $('.view-cloudwords-translatable .vbo-select:enabled').each(function() {
            output.push(this.value);
          });
          Drupal.cloudwords.updateChecked(display, op, output);
        });
      });

      $('.view-display-id-' + display + ' .views-table tbody tr td').once('clowdwords-row-click', function() {
        $(this).click(function(event) {
          if (event.target.type !== 'checkbox' && !$(event.target).is('a')) {
            var checkbox = $(this).parent().find(':checkbox');
            if (!checkbox.attr('disabled')) {
              checkbox.trigger('click').trigger('change');
            }
          }
        });
      });

      $('.view-display-id-' + display + ' .views-reset-button input').once('clowdwords-reset-click', function() {
        $(this).click(function(e) {
          $(this).closest('form').find('input[type=text]').val('');
          setTimeout(function() {
            $('#edit-submit-cloudwords-translatable').trigger('click');
          }, 200);
        });
      });
    });
    $('#edit-reset', context).once(function() {
      $(this).click(function(event) {
        event.preventDefault();
        $('.views-exposed-widgets select', context).val('All').change();
        $('.views-exposed-widgets #edit-label', context).val('');
      });
    });

    $('#cloudwords-project-language-import-form #edit-select-all', context).once(function() {
      $(this).click(function(event) {
        $select_state = $(this).is(':checked');
        var $form = $(this).closest('form');
        $form.find('#edit-node-status input.form-checkbox').each(function(index, el) {
          if($select_state === true){
            $(el).attr('checked', true);
          }else{
            $(el).attr('checked', false);
          }
        })
      });
    });    

  }
};

})(jQuery);
