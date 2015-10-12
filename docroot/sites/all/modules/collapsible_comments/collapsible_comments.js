(function ($) {
  // Drupal behaviour for collapsing indented comments.
  Drupal.behaviors.collapsibleComments = {
    attach: function (context, settings) {
      // Cache selections and return early if appropiate.
      var $comments = $('#comments:not(.collapsible-comments-processed)').addClass('collapsible-comments-processed');
      if ($comments.size() < 1) return;
      var $indented = $comments.find('> .indented');
      if ($indented.size() < 1) return;

      // Our settings.
      var level = settings.collapsible_comments.level;
      var mode = settings.collapsible_comments.mode;
      var effect = settings.collapsible_comments.effect;

      // Theme function for the show/hide links
      // Make sure to keep a.comment-thread-expand if you override this theme function
      Drupal.theme.prototype.commentCollapseLink = function(text) {
        return '<a href="#" class="comment-thread-expand">'+ Drupal.t(text) +'</a>';
      }
      var button = Drupal.theme('commentCollapseLink', 'Show responses');

      // 1. Find the appropiate indentation level
      $toProcess = collapsibleCommentsGetLevel(level, $comments);
      // 2. Execute the proper setup depending on mode
      collapsibleCommentsEnable($toProcess, button, mode)

      // 3. Bind our behaviour to the click event
      $('.comment-thread-expand', $comments).click(function(){
        var $this = $(this);
        var $parent = $this.parent();
        var $toToggle = $parent.next('.indented');
        var text = ($this.text() == Drupal.t('Hide responses')) ? Drupal.t('Show responses') : Drupal.t('Hide responses');
        $this.text(text);

        if (effect == 'slide') {
          $toToggle.slideToggle();
        }

        else if (effect == 'hide') {
          $toToggle.toggle();
        }

        $parent.toggleClass('indented-hidden');

        return false;
      }); // End click event

      /**
       * Helper function to enable a collasible comment thread.
       */
      function collapsibleCommentsEnable(element, button, mode) {
        element.hide();
        element.prev().append(button).addClass('indented-hidden collaspsible-comments-enabled');
        if (mode == 0) return;

        if (mode == 1) {
          // handle all children indented independently
          var $subIndent =  $('.indented', element);
          var num = $subIndent.size();
          if (num < 1) return;

          $subIndent.each(function(){
            var $this = $(this);
            $this.hide();
            $this.prev().append(button).addClass('indented-hidden collaspsible-comments-enabled');
          });
        } // End mode 1
      }

      /**
       * Helper function to select the appropiate level of indentation
       */
      function collapsibleCommentsGetLevel(level,comments){
        var currentLevel = 0;
        $selection = $('> .indented', comments);
        while (currentLevel < level) {
          $selection = $('> .indented', $selection);
          currentLevel++;
        }
        return $selection;
      }

    } // End attach
  } // End behavior collapsibleComments
})(jQuery);
