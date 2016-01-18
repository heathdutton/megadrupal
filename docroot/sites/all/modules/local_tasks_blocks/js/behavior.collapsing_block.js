(function ($) {
  Drupal.behaviors.local_tasks_blocks = {
    attach: function (context, settings) {
      var block_settings = settings.local_tasks_blocks || {};
      for (var delta in block_settings) {
        var settings = block_settings[delta];
        try {
          if (settings.collapsing_block) {
            // Delta will have underscores, but the block ID will have hyphens.
            $('#block-local-tasks-blocks-' + delta.replace(/\_/g, '-'), context).once('local_tasks_blocks_collapsing_block', function () {
              // Store a local copy of this delta.
              var _delta = delta;
              var block = this,
                $block = $(block),
                $handle = $('<div class="local-tasks-blocks-handle"></div>').appendTo($block),
                $container = $('<div class="local-tasks-blocks-container"></div>').appendTo($block),
                $block_content = $handle.prevAll(),
                block_title = $block.find('.block-title').text() || Drupal.t('Tasks'),
                $trigger = $('<a href="#" class="local-tasks-blocks-trigger" />').appendTo($handle);
              $block_content.each(function() {
                $(this).detach().prependTo($container);
              });
              var trigger_text = {
                'show': Drupal.t('Show @block_title', { '@block_title' : block_title }),
                'hide': Drupal.t('Hide @block_title', { '@block_title' : block_title })
              };
              $trigger.text(trigger_text.show);
              $trigger.click(function () {
                $container.toggleClass('local-tasks-blocks-container-active');
                $block.toggleClass('local-tasks-blocks-block-active');
                $trigger.toggleClass('local-tasks-blocks-trigger-active');
                // Flip the state:
                var state = !$(this).data('trigger-active');
                // Update the start_expanded-triggering cookie:
                if ($.cookie) {
                  $.cookie(
                    'local_tasks_blocks_user_state_' + _delta,
                    // cookie value ("true"|"false"):
                    state.toString(),
                    // cookie options:
                    {
                      // expiration in days from now (1 year):
                      'expires': 365,
                      'path': '/'
                    }
                  );
                }
                // Store the status on the trigger element:
                $(this).data('trigger-active', state);
                // Flip the text to reflect the state:
                $trigger.text(state ? trigger_text.hide : trigger_text.show);

                return false;
              });
              if (settings.vanishing_trigger) {
                $block.addClass('local-tasks-blocks-block-vanishing');
              }
              if (settings.notext_trigger) {
                $trigger.addClass('local-tasks-blocks-trigger-notext');
              }
              // Determine if the block should be expanded at the outset:
              var expand_block = false;
              if (settings.start_expanded) {
                // If the block should default to expanded, check if the user has
                // collapsed it (stored as a cookie).  Either the cookie doesn't
                // exist, or it's "true"|"false"; if it's unset or true, then
                // we default to expanded:
                if ($.cookie && $.cookie('local_tasks_blocks_user_state_' + _delta) != 'false') {
                  expand_block = true;
                }
              }
              // "Keep Expanded" overrides the "Sart Expanded" default behavior,
              // as it is more specific than an overall strategy:
              if (settings.prevent_collapse_on_tabchange) {
                if ($.cookie && $.cookie('local_tasks_blocks_keep_expanded_' + _delta) == 'true') {
                  expand_block = true;
                  // This cookie is evaluated when *the next* URL is loaded, so
                  // it has done its job of instructing us to keep the block
                  // expanded. Now, we need to prevent future requests from also
                  // expanding, unless they are between primary tabs.
                  // Clear the cookie (let tabs revert to default behavior):
                  $.cookie(
                    'local_tasks_blocks_keep_expanded_' + _delta,
                    // cookie value (null clears the cookie):
                    null,
                    // cookie options:
                    {
                      // expiration in days from now (negative clears cookie):
                      'expires': -1,
                      'path': '/'
                    }
                  );
                }

                $('.tabs.primary a, .menu li:not(.leaf) > a', $block).click(function(e) {
                  // Set the cookie (remember to expand block on next page):
                  if ($.cookie) {
                    $.cookie(
                      'local_tasks_blocks_keep_expanded_' + _delta,
                      // cookie value ("true" = expand block on next page):
                      'true',
                      // cookie options:
                      {
                        // expiration in days from now (null = session cookie):
                        'expires': null,
                        'path': '/'
                      }
                    );
                  }
                });
              }
              // Expand the block by clicking its trigger
              if (expand_block) {
                $trigger.click();
              }
            });
          }
        } catch(e) { }
      }
    }
  }
})(jQuery);
