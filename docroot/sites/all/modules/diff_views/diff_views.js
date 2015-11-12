(function ($) {

  Drupal.behaviors.diffViewsRevisions = {
    attach: function (context, settings) {
      // Redirect to Diff page after the button was clicked.
      $('input.diff-views-submit', context).once('diff-views').click(function (e) {
        e.preventDefault();
        var form = $(this).closest('form');
        var n = $('input[name="diff_views_radio[new]"]:radio:checked', form).val();
        var o = $('input[name="diff_views_radio[old]"]:radio:checked', form).val();
        if (!n || !o) {
          alert(Drupal.t('Select 2 revisions to compare.'));
          return false;
        }
        n = n.split(':');
        o = o.split(':');
        if (n[0] != o[0]) {
          alert(Drupal.t(t('Only revisions from the same entity can be compared.')));
        }
        else if (n[1] == o[1]) {
          alert(Drupal.t(t('Only revisions from the same entity can be compared.')));
        }
        else {
          window.location = Drupal.settings.basePath + 'node/' + o[0] + '/revisions/view/' + o[1] + '/' + n[1];
        }
      });

      // Port Diff row highlighting.
      $('.diff-views-rows-js').once('diff-views', function() {
        var $table = this;
        if (!$('input[name="diff_views_radio[new]"]:checked, input[name="diff_views_radio[old]"]:checked', $table).length) {
          // Set default revisions.
          $('input[name="diff_views_radio[new]"]:first', $table).attr('checked', 'checked');
          $('input[name="diff_views_radio[old]"]:eq(1)', $table).attr('checked', 'checked');
        }

        Drupal.behaviors.diffViewsRevisions.updateDiffRadios($table);
        $('input[name="diff_views_radio[new]"], input[name="diff_views_radio[old]"]', this).click(function() {
          Drupal.behaviors.diffViewsRevisions.updateDiffRadios($table);
        });
      });
    },

    'updateDiffRadios': function($table) {
      var $rows = $('tr', $table);

      var newTd = false;
      var oldTd = false;
      if (!$rows.length) {
        return true;
      }
      $rows.removeClass('selected').each(function () {
        var $row = $(this);
        $row.removeClass('selected');
        var $inputs = $row.find('input[type="radio"]');
        var $oldRadio = $inputs.filter('[name="diff_views_radio[old]"]').eq(0);
        var $newRadio = $inputs.filter('[name="diff_views_radio[new]"]').eq(0);
        if (!$oldRadio.length || !$newRadio.length) {
          return true;
        }
        if ($oldRadio.attr('checked')) {
          oldTd = true;
          $row.addClass('selected');
          $oldRadio.css('visibility', 'visible');
          $newRadio.css('visibility', 'hidden');
        } else if ($newRadio.attr('checked')) {
          newTd = true;
          $row.addClass('selected');
          $oldRadio.css('visibility', 'hidden');
          $newRadio.css('visibility', 'visible');
        } else {
          if (Drupal.settings.diffRevisionRadios == 'linear') {
            if (newTd && oldTd) {
              $oldRadio.css('visibility', 'visible');
              $newRadio.css('visibility', 'hidden');
            } else if (newTd) {
              $newRadio.css('visibility', 'visible');
              $oldRadio.css('visibility', 'visible');
            } else {
              $newRadio.css('visibility', 'visible');
              $oldRadio.css('visibility', 'hidden');
            }
          } else {
            $newRadio.css('visibility', 'visible');
            $oldRadio.css('visibility', 'visible');
          }
        }
      });

      return true;
    }
  };

})(jQuery);
