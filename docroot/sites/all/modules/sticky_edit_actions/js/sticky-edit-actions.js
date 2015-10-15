(function($) {
  /**
   * This entire SIAF basically just adds a class in 3 different cases.
   */
  Drupal.behaviors.stickyEditActions = {
    attach: function(context, settings) {
      // This fires the waypoint on the element normally being at the bottom
      // of the viewport.
      $('.my-sticky-element', context).waypoint('sticky', {
        offset: 'bottom-in-view',
        stuckClass: 'unstuck'
      });
      // This fires the waypoint when fieldsets are expanded, since Drupal
      // uses $.slideDown(), which has a callback which ends up using a
      // window.scrollTo() to do the expansion.
      // This fires the waypoint when vertical tabs are clicked.
      $('.vertical-tab-button a').click(function() {
        $.waypoints('refresh');
      });
      // Set dimensions of .my-sticky-element. We must do
      // this because once we 'position: fixed' in CSS it comes out of the DOM
      // flow and is uncontrollable by percentage.
      function setStickyElementWidthAndParentHeight() {
        $('.my-sticky-element').each(function (e) {
          $this = $(this);
          $this.width( $this.parent().width() );
          $this.parent().height( $this.outerHeight() );
        });
      }
      // First set dimensions on page load.
      setStickyElementWidthAndParentHeight();
      // Then continue to set the dimensions on resize events.
      $(window).resize(setStickyElementWidthAndParentHeight);
    }
  };

  /**
   * OVERRIDING this function from collapse.js so we can add some
   * callbacks to slideDown & slideUp(). The .click() handler will not work on
   * fieldsets.
   *
   * Toggle the visibility of a fieldset using smooth animations.
   */
  Drupal.toggleFieldset = function (fieldset) {
    var $fieldset = $(fieldset);
    if ($fieldset.is('.collapsed')) {
      var $content = $('> .fieldset-wrapper', fieldset).hide();
      $fieldset
        .removeClass('collapsed')
        .trigger({ type: 'collapsed', value: false })
        .find('> legend span.fieldset-legend-prefix').html(Drupal.t('Hide'));
      $content.slideDown({
        duration: 'fast',
        easing: 'linear',
        complete: function () {
          Drupal.collapseScrollIntoView(fieldset);
          fieldset.animating = false;
          $.waypoints('refresh');
        },
        step: function () {
          // Scroll the fieldset into view.
          Drupal.collapseScrollIntoView(fieldset);
        }
      });
    }
    else {
      $fieldset.trigger({ type: 'collapsed', value: true });
      $('> .fieldset-wrapper', fieldset).slideUp('fast', function () {
        $fieldset
          .addClass('collapsed')
          .find('> legend span.fieldset-legend-prefix').html(Drupal.t('Show'));
        fieldset.animating = false;
        $.waypoints('refresh');
      });
    }
  };
}) (jQuery);


