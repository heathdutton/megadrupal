/**
 * @file
 * Contains LocalStorageWysiwyg class.
 */

/**
 * Local Storage Wysiwyg plugin.
 *
 * @param $el
 * @constructor
 */
function LocalStorageWysiwyg($el) {
  this.$el = $el;
  this.id = $el.attr('id');
  this.attachEvents();
}

(function ($) {
  LocalStorageWysiwyg.prototype = {
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
            if (CKEDITOR.instances[me.id]) {
              CKEDITOR.instances[me.id].on('key', function (e) {
                me.html5ls.startCollecting();
                e.removeListener();
              });
            }
          }

          if (typeof tinyMCE != 'undefined') {
            tinyMCE.onAddEditor.add(function (mgr, editor) {
                if (editor.id == me.id) {
                  var callback = function () {
                    me.html5ls.startCollecting();
                    editor.onKeyPress.remove(callback);
                  };
                  editor.onKeyPress.add(callback);
                }
              }
            );
          }

          var epiceditor = me.$el.data('epiceditor');
          if (typeof EpicEditor != 'undefined' && epiceditor) {
            var callback = function () {
              me.html5ls.startCollecting();
              epiceditor.removeListener('update', callback);
            };
            epiceditor.on('update', callback);
          }
        },

        'html5ls:getElementValue': function (e, scope, key, id) {
          if (Drupal.wysiwyg.instances[id] && Drupal.wysiwyg.instances[id].status) {
            switch (Drupal.wysiwyg.instances[id].editor) {
              case 'epiceditor':
                scope.value = this.$el.data('epiceditor')
                  .getElement('editor').body.innerHTML;
                break;

              default:
                scope.value = Drupal.wysiwyg.instances[id].getContent();
            }
          }
        },

        'html5ls:setElementValue': function (e, scope, key, id, value) {
          if (Drupal.wysiwyg.instances[id] && Drupal.wysiwyg.instances[id].status) {
            switch (Drupal.wysiwyg.instances[id].editor) {
              case 'epiceditor':
                this.$el.data('epiceditor')
                  .getElement('editor').body.innerHTML = value;
                break;

              default:
                Drupal.wysiwyg.instances[id].setContent(value);
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
      },

      onChangeHandlers: {
        'ckeditor': function () {
          var me = this;
          if (typeof CKEDITOR != 'undefined') {
            CKEDITOR.on('instanceReady', function (e) {
              // CKEditor integration.
              if (e.editor.name == me.id) {
                CKEDITOR.instances[me.id].on('key', function (e) {
                  me.$el.html5ls().startCollecting();
                  e.removeListener();
                });
              }
            });
          }
        },

        'tinymce': function () {
          var me = this;
          if (typeof tinyMCE != 'undefined') {
            tinyMCE.onAddEditor.add(function (mgr, editor) {
                if (editor.id == me.id) {
                  var callback = function () {
                    me.$el.html5ls().startCollecting();
                  };
                  editor.onKeyPress.add(callback);
                }
              }
            );
          }
        },

        'epiceditor': function () {
          var me = this;
          if (typeof EpicEditor != 'undefined') {
            var hInterval = setInterval(function () {
              if (me.$el.data('epiceditor')) {
                me.$el.data('epiceditor').on('update', function () {
                    me.$el.html5ls().startCollecting();
                  }
                );

                clearInterval(hInterval);
              }
            }, 100);
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

        $.each(me.events.onChangeHandlers, function (i, callback) {
          callback.bind(me).call();
        });

        $.each(me.events.html5ls, function (name, callback) {
          me.$el.bind(name, callback.bind(me));
        });
      });
    },

    /**
     * Define setState callback for each editor type.
     */
    setStateCallbacks: {
      ckeditor: function (editor, state) {
        editor.getCommand('html5ls').setState(
          state
            ? CKEDITOR.TRISTATE_OFF
            : CKEDITOR.TRISTATE_ON
        );
      },

      tinymce3: function (editor, state) {
        var controlId = editor.id + '_html5ls';
        editor.controlManager.controls[controlId].setActive(!state);
      },

      epiceditor: function () {

      }
    },

    /**
     * Get editor instance.
     */
    getInstance: function () {
      if (!Drupal.wysiwyg.instances[this.id]) {
        return false;
      }

      var instance = {
        type: Drupal.wysiwyg.instances[this.id].editor,
        editor: undefined
      };

      switch (instance.type) {
        case 'ckeditor':
          instance.editor = CKEDITOR.instances[this.id];
          break;

        case 'tinymce':
          instance.type += tinyMCE.majorVersion;
          instance.editor = tinyMCE.get(this.id);
          break;

        case 'epiceditor':
          instance.editor = this.$el.data('epiceditor');
          break;

        case 'none':
          break;

        default:
          throw new Error('Unknown editor type.');
      }

      return instance;
    },

    /**
     * Set button state.
     *
     * @param [state]
     */
    setState: function (state) {
      var instance = this.getInstance();
      if (!instance || !instance.editor) {
        return false;
      }
      if (this.setStateCallbacks.hasOwnProperty(instance.type)) {
        if (state === undefined) {
          state = this.html5ls.getState();
        }

        return this.setStateCallbacks[instance.type](instance.editor, state);
      }
      else {
        throw new Error('Unknown editor type.');
      }
    },

    /**
     * Plugin attach callback.
     */
    attach: function () {
      var instance = this.getInstance();

      this.setState(this.html5ls.getState());
      if (instance.type != 'epiceditor') {
        if (this.$el.html5lsPlain) {
          this.$el.html5lsPlain().hide();
        }
      }
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
        var instance = me.getInstance();

        switch (instance.type) {
          case 'none':
          case 'epiceditor':
            if (me.$el.html5lsPlain) {
              me.$el.html5lsPlain().show().class(instance.type);
            }
            break;

          default:
            break;
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
    }
  };

  /**
   * jQuery plugin to help to work with LocalStorageWysiwyg class.
   *
   * @returns {LocalStorageWysiwyg}
   * @constructor
   */
  $.fn.html5lsWysiwyg = function () {
    var html5lsWysiwyg = this.data('html5lsWysiwyg');
    if (!html5lsWysiwyg) {
      html5lsWysiwyg = new LocalStorageWysiwyg(this);
      this.data('html5lsWysiwyg', html5lsWysiwyg);
    }

    return html5lsWysiwyg;
  };
})(jQuery);
