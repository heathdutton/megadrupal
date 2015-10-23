/**
 * @file
 * Contains LocalStorageMessage class.
 *
 * Each (Local Storage) form element element can issue these events (hooks):
 *
 * html5ls:setMessage:before
 *    Description:
 *      Before the message is displayed.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} msg - Message to be displayed.
 *        @property {String} type - Type of the message (status, warning, error).
 *      @params {String} message
 *      @params {String} type
 *
 * html5ls:setMessage
 *    Description:
 *      Display the message.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} msg - Message to be displayed.
 *        @property {String} type - Type of the message (status, warning, error).
 *      @params {String} message
 *      @params {String} type
 *
 * html5ls:setMessage:after
 *    Description:
 *      After the message is displayed.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *      @params {String} message
 *      @params {String} type
 *
 * Variable "scope" is an object that is used to share data between
 * LocalStorage class and interactors. Some hooks expect to get data from
 * its properties.
 */

/**
 * Local Storage Message plugin.
 *
 * @param $el
 * @constructor
 */
function LocalStorageMessage($el) {
  this.$el = $el;
  this.attachEvents();
}

(function ($) {
  /**
   * Template for Local Storage messages.
   *
   * Displayed before HTML5LS-active elements to provide more information
   * about what's going on to a user.
   *
   * @param msg
   *    Message to output.
   * @param type
   *    Message type: status, warning, error. By default "status" type will
   *    be used.
   * @returns string
   *    HTML temlate of a message.
   */
  Drupal.theme.prototype.localStorageMsg = function (msg, type) {
    type = type || 'status';

    var classes = ['messages', 'html5ls-message', type];
    return '<div class="' + classes.join(' ') + '">'
      + msg
      + '</div>';
  };

  LocalStorageMessage.prototype = {
    /**
     * @type LocalStorage
     */
    html5ls: undefined,
    /**
     * @type jQuery
     */
    $el: undefined,
    /**
     * @type jQuery
     */
    $message: undefined,
    /**
     * Events to attach to the form element.
     */
    events: {
      /**
       * LocalStorage events to hook on.
       */
      html5ls: {
        'html5ls:save:after': function () {
          this.setMessage(Drupal.t('Local Storage: saved'));
        },

        'html5ls:setRestoredValue:after': function (e, scope, restoredValue, defaultValue) {
          if (restoredValue && restoredValue != defaultValue) {
            this.setMessage(Drupal.t('Restored value'));
          }
        },

        'html5ls:setDefaultValue:after': function (e, scope, restoredValue, defaultValue) {
          if (defaultValue && defaultValue != restoredValue) {
            this.setMessage(Drupal.t('Default value'));
          }
        },

        'html5ls:setMessage': function (e, scope, msg, type) {
          var $output = $(Drupal.theme('localStorageMsg', msg, type));
          if (this.$message) {
            this.$message.replaceWith($output);
          }
          else {
            $output.insertBefore(this.$el).hide().fadeIn('slow');
          }
          this.$message = $output;
        }
      }
    },

    /**
     * Attach plugin events.
     */
    attachEvents: function () {
      var me = this;

      me.$el.bind('html5ls:init', function () {
        me.html5ls = me.$el.html5ls();

        $.each(me.events.html5ls, function (name, callback) {
          me.$el.bind(name, callback.bind(me));
        });
      });
    },

    /**
     * Set status message.
     *
     * @param msg
     *    Message to output.
     * @param type
     *    Type of the message.
     */
    setMessage: function (msg, type) {
      // Allow to interact with message and type.
      var scope = this.html5ls.invoke('html5ls:setMessage:before', msg, type);
      if (scope.msg) {
        msg = scope.msg;
      }
      if (scope.type) {
        type = scope.type;
      }

      // Allow interactors to show message in any way they want.
      scope = this.html5ls.invoke('html5ls:setMessage', msg, type);

      // Allow interactors to react after message is shown.
      this.html5ls.invoke('html5ls:setMessage:after', scope.msg, scope.type);
    }
  };

  /**
   * jQuery plugin to help to work with LocalStorageMessage class.
   *
   * @returns {LocalStorageMessage}
   * @constructor
   */
  $.fn.html5lsMessage = function () {
    var html5lsMessage = this.data('html5lsMessage');
    if (!html5lsMessage) {
      html5lsMessage = new LocalStorageMessage(this);
      this.data('html5lsMessage', html5lsMessage);
    }

    return html5lsMessage;
  };
})(jQuery);
