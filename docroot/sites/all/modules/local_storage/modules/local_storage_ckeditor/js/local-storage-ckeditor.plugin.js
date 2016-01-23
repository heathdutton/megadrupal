/**
 * @file
 * Contains LocalStorageCkeditor class.
 */

/**
 * Local Storage Ckeditor plugin.
 *
 * @param $el
 * @constructor
 */
function LocalStorageCkeditor($el) {
  this.$el = $el;
  this.id = $el.attr('id');
  this.attachEvents();
}

(function ($) {
  LocalStorageCkeditor.prototype = {
    /**
     * @type LocalStorage
     */
    html5ls: undefined,
    /**
     * @type String
     */
    id: '',
    /**
     * Events to attach to the form element.
     */
    events: {
      /**
       * LocalStorage events to hook on.
       */
      html5ls: {
        'html5ls:save:after': function () {
          this.setState();
        },

        'html5ls:setDefaultValue:after': function () {
          var me = this;
          if (typeof CKEDITOR != 'undefined') {
            if (CKEDITOR.instances) {
              if (CKEDITOR.instances[me.id]) {
                CKEDITOR.instances[me.id].on('key', function (e) {
                  me.html5ls.startCollecting();
                  e.removeListener();
                });
              }
            }
          }
        },

        'html5ls:getElementValue': function (e, scope, key, id) {
          if (typeof CKEDITOR != 'undefined') {
            if (CKEDITOR.instances) {
              if (CKEDITOR.instances[id]) {
                scope.value = CKEDITOR.instances[id].getData();
              }
            }
          }
        },

        'html5ls:setElementValue': function (e, scope, key, id, value) {
          if (typeof CKEDITOR != 'undefined') {
            if (CKEDITOR.instances) {
              if (CKEDITOR.instances[id]) {
                CKEDITOR.instances[id].setData(value);
              }
            }
          }
        },

        'html5ls:showPlainBtn': function (e, scope, $el) {
          if ($el.siblings('.wysiwyg-toggle-wrapper:visible').size()) {
            $el.addClass('with-wysiwyg-toggle-wrapper');
          }
          else {
            $el.removeClass('with-wysiwyg-toggle-wrapper');
          }
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

        if (typeof CKEDITOR != 'undefined') {
          CKEDITOR.on('instanceReady', function (e) {
            // CKEditor integration.
            if (e.editor.name == me.id) {
              CKEDITOR.instances[me.id].on('key', function (e) {
                me.html5ls.startCollecting();
                e.removeListener();
              });
            }
          });
        }

        $.each(me.events.html5ls, function (name, callback) {
          me.$el.bind(name, callback.bind(me));
        });
      });
    },

    /**
     * Plugin detach callback.
     */
    detach: function () {
      var me = this;

      if (me.detachTimeout) {
        clearTimeout(me.detachTimeout);
      }

      // Set timeout in order to exclude spurious calls of detach method.
      this.detachTimeout = setTimeout(function () {
        if (me.$el.html5lsPlain) {
          me.$el.html5lsPlain().show().class('ckeditor');
        }
      }, 100);
    },

    /**
     * Plugin invoke callback.
     */
    invoke: function () {
      if (this.html5ls.getState()) {
        this.html5ls.setRestoredValue();
      }
      else {
        this.html5ls.setDefaultValue();
      }

      this.setState();
    },

    /**
     * Set button state.
     *
     * @param [state]
     */
    setState: function (state) {
      if (typeof CKEDITOR != 'undefined') {
        if (state === undefined) {
          state = this.html5ls.getState();
        }
        if (CKEDITOR.instances[this.id]) {
          CKEDITOR.instances[this.id].getCommand('html5ls').setState(
            state ? CKEDITOR.TRISTATE_OFF : CKEDITOR.TRISTATE_ON
          );
        }
      }
    }
  };

  /**
   * jQuery plugin to help to work with LocalStorageCkeditor class.
   *
   * @returns {LocalStorageCkeditor}
   * @constructor
   */
  $.fn.html5lsCkeditor = function () {
    var html5lsCkeditor = this.data('html5lsCkeditor');
    if (!html5lsCkeditor) {
      html5lsCkeditor = new LocalStorageCkeditor(this);
      this.data('html5lsCkeditor', html5lsCkeditor);
    }

    return html5lsCkeditor;
  };
})(jQuery);
