/**
 * @file
 * Contains Comment Easy Reply qTip javascript functions.
 */

(function ($) {
  Drupal.behaviors.comment_easy_reply_qtip = {
    attach: function(context, settings) {
      var comment_form = $("#comment-form #edit-comment-body");
      var comment_subject_form = $("#comment-form #edit-subject");
      $('.comment-easy-reply-number-link-wrapper', context).once('comment-easy-reply-qtip').each(function () {
        var tips = $(this).children('.comment-easy-reply-number-link-tips');
        var link = $(this).children('.comment-easy-reply-number-link');
        if (Drupal.settings.comment_easy_reply != 'undefined'
          && Drupal.settings.comment_easy_reply.replytip_activated) {
          Drupal.comment_easy_reply_qtip.build_qtip(link, tips.html());
        }    
      });
      $('.comment-easy-reply-referrer-link-wrapper', context).once('comment-easy-reply-qtip').each(function () {
        $(this).find('.comment-easy-reply-number-link').each(function () {
          var commentnumber = Drupal.comment_easy_reply.get_number_from_class($(this), 'comment-easy-reply-linknum-');
          if (commentnumber != 'undefined') {
            var selector = '.comment-easy-reply-referrer-tips-' + commentnumber;
            var add_class = Drupal.comment_easy_reply.get_add_class($(this));
            if (add_class) {
              selector += '.comment-easy-reply-added-' + add_class;
            }
            else {
              selector += '.comment-easy-reply-added-noclass';
            }
            var tips = $(this).closest('.comment').find(selector);
            if (tips != 'undefined') {
              Drupal.comment_easy_reply_qtip.build_qtip(this, tips.html());
            }
          }
        });
      });
    }
  };
  Drupal.comment_easy_reply_qtip = {
    build_qtip: function(el, tip_text){
      // Set delay on click to immediate
      if (Drupal.settings.qtip.show_event_type == 'click') {
        show_delay = 1;
      }
      else {
        show_delay = 30; // The default qTip value is 140, set for hover links
      }      
      // Determine how to position the tooltip
      if (Drupal.settings.qtip.show_speech_bubble_tip) {
        // Set the proper qtip speech-bubble tip position
        if (Drupal.settings.qtip.show_speech_bubble_tip_side) {
          switch_position = Drupal.settings.qtip.tooltip_position.split('_');
          // We do not want to put tooltip on it's side if it is being displayed
          // in the center or side of an element
          if (switch_position[1] == 'center') {
            tip_position = Drupal.settings.qtip.tooltip_position;
          }
          else {
            tip_position = switch_position[1] + ' ' + switch_position[0];
          }
        }
        // Otherwise just set it to the same value as the tooltip
        else {
          tip_position = Drupal.settings.qtip.tooltip_position;
        }
      }
      else {
        tip_position = false;
      }      
      // Is a custom style declared by the admin?
      if(Drupal.settings.qtip.color == 'custom-color') {
        Drupal.settings.qtip.color = Drupal.settings.qtip.custom_color;
      }      
      // Do we want a shadow to show under the tooltip?
      if(Drupal.settings.qtip.show_shadow) {
        style_classes = 'ui-tooltip-shadow ' + Drupal.settings.qtip.color;
      }
      else {
        style_classes = Drupal.settings.qtip.color;
      }      
      // Do we want to show the tooltips with rounded corners?
      if(Drupal.settings.qtip.rounded_corners) {
        style_classes += ' ui-tooltip-rounded';
      }      
      //Do we want a solid tip to display
      if (Drupal.settings.qtip.show_speech_bubble_tip_solid) {
        solid_tip = false;
      }
      else {
        solid_tip = 5;
      }
      set_my = Drupal.settings.qtip.tooltip_position;
      set_at = Drupal.settings.qtip.target_position;
      show_event = Drupal.settings.qtip.show_event_type;
      hide_event = Drupal.settings.qtip.hide_event_type;
      $(el).qtip({
        content: {
          text: tip_text
        },
        position: {
          my: set_my, // my = speech bubble position on tooltip
          at: set_at, // at = where on link text tooltip will appear
          adjust: {
            screen: true // Keeps tooltip within visible window
          }
        },
        style: {
          classes: style_classes,
          tip: {
            corner: tip_position, // Position of speech bubble tip...false will not display tip
            border: solid_tip, // parseInt(Drupal.settings.qtip.border_width)
            width: parseInt(Drupal.settings.qtip.speech_bubble_size),
            height: parseInt(Drupal.settings.qtip.speech_bubble_size)
          }
        },
        show: {
          event: show_event,
          solo: true, // Determines whether or not the tooltip will hide all others when the show.event is triggered on the show.target. 
          delay: show_delay
        },
        hide: {
          event: hide_event,
          fixed: true //When set to true, the tooltip will not hide if moused over, allowing the contents to be clicked and interacted with.
        }
      });
    }
  };
})(jQuery);
