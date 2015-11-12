/**
 * @file Contains LocalStorage class.
 *
 * Each (Local Storage) form element element can issue these events (hooks):
 *
 * html5ls:init
 *    Description:
 *      Local Storage is initialized.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *
 * html5ls:collect
 *    Description:
 *      Collect data from element, called every 300 msec by default.
 *    Arguments:
 *      @params {Object} event
 *
 * html5ls:setDefaultValue:before
 *    Description:
 *      Before default value is set to the element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} defaultValue - Default value of the element.
 *      @params {String} defaultValue
 *
 * html5ls:setDefaultValue:after
 *    Description:
 *      After default value is set to the element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *      @params {String} defaultValue
 *      @params {String} restoredValue
 *
 * html5ls:setRestoredValue:before
 *    Description:
 *      Before saved value is set in the element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *      @params {Object} scope
 *        @property {String} restoredValue - Restored value of the element.
 *      @params {String} restoredValue
 *
 * html5ls:setRestoredValue:after
 *    Description:
 *      After saved value is set in the element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} restoredValue - Restored value of the element.
 *      @params {String} restoredValue
 *      @params {String} defaultValue
 *
 * html5ls:getElementValue
 *    Description:
 *      Extract from element and provide to LocalStorage a value.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} value - Value of the element.
 *      @params {String} key - Key of the current storage.
 *      @params {String} elementId - Id of the form element.
 *
 * html5ls:save:before
 *    Description:
 *      Before the data is saved to LocalStorage.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} value - Value of the element.
 *      @params {String} key - Key of the current storage.
 *      @params {String} elementId - Id of the form element.
 *      @params {String} value - Value of the form element.
 *
 * html5ls:save:after
 *    Description:
 *      After the data is saved to LocalStorage.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *      @params {String} key - Key of the current storage.
 *      @params {String} elementId - Id of the form element.
 *      @params {String} value - Value of the form element.
 *
 * html5ls:setElementValue:before
 *    Description:
 *      Before the value is set to element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} value - Value of the element.
 *      @params {String} key - Key of the current storage.
 *      @params {String} elementId - Id of the form element.
 *      @params {String} value - Value of the form element.
 *
 * html5ls:setElementValue
 *    Description:
 *      Set the value is to the element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} value - Value of the element.
 *      @params {String} key - Key of the current storage.
 *      @params {String} elementId - Id of the form element.
 *      @params {String} value - Value of the form element.
 *
 * html5ls:setElementValue:after
 *    Description:
 *      After the value is set to the element.
 *    Arguments:
 *      @params {Object} event
 *      @params {Object} scope
 *        @property {String} value - Value of the element.
 *      @params {String} key - Key of the current storage.
 *      @params {String} elementId - Id of the form element.
 *      @params {String} value - Value of the form element.
 *
 * Each LocalStorage plugin can provide own events, check them in their
 * source files.
 */

/**
 * Local Storage class.
 *
 * Handles interactions between input element and LocalStorageCore class.
 *
 * @param {jQuery} $el
 * @param {String} key
 * @param {LocalStorageCore} Storage
 * @param {Object} settings
 * @constructor
 */
function LocalStorage($el, Storage, key, settings) {
  this.$el = $el;

  this.key = key;
  this.id = $el.attr('id');
  this.settings = jQuery.extend({
    default: 1,
    expire: 48,
    plugins: {}
  }, settings || {});

  /**
   * @type LocalStorageCore
   */
  this.Storage = Storage;

  this.defaultValue = $el.val();
  this.restoredValue = this.getData().value;
  this.showDefault = this.settings.default;
  this.stopCollecting();
}

(function ($) {
  LocalStorage.prototype = {
    /**
     * @type jQuery - Form element
     */
    $el: undefined,
    /**
     * @type String - Id of the form element.
     */
    id: '',
    /**
     * @type String - LocalStorage identifier.
     */
    key: '',
    /**
     * @type Object - Local Storage settings.
     */
    settings: {},
    /**
     * @type LocalStorageCore - Instance of the LocalStorageCore.
     */
    Storage: undefined,
    /**
     * @type String - Default value of the form element.
     */
    defaultValue: '',
    /**
     * @type String - Restored (saved) value of the form element.
     */
    restoredValue: '',
    /**
     * @type Boolean - Current LocalStorage state of the form element.
     */
    showDefault: false,
    /**
     * @type Boolean - Collect is in process of not.
     */
    collect: false,

    /**
     * Initialize LocalStorage.
     */
    init: function () {
      var me = this;
      // Start collecting data on first change event.
      me.$el.bind('keypress change', function () {
        me.startCollecting();
      });

      // Bind html5ls events.
      $.each(me.events.html5ls, function (name, callback) {
        me.$el.bind(name, callback.bind(me));
      });
      this.invoke('html5ls:init');
    },

    /**
     * Events to attach to the form element.
     */
    events: {
      /**
       * LocalStorage events to hook on.
       */
      html5ls: {
        'html5ls:init': function () {
          if (this.showDefault) {
            this.setDefaultValue();
          }
          else {
            this.setRestoredValue();
          }
        },

        'html5ls:collect': function () {
          this.collectValue();
        },

        'html5ls:save:after': function () {
          if (this.showDefault) {
            this.showDefault = false;
          }
        }
      }
    },

    /**
     * Set default value (before restoring from localStorage) to the element.
     *
     * @returns {*}
     */
    setDefaultValue: function () {
      var defaultValue = this.defaultValue;
      var scope = this.invoke('html5ls:setDefaultValue:before', defaultValue);
      if (scope.defaultValue !== undefined) {
        defaultValue = scope.defaultValue;
      }

      this.showDefault = true;

      var data = this.getData();
      this.restoredValue = this.showDefault && data.value != this.getElementValue()
        ? data.value
        : this.getElementValue();

      if (this.collect) {
        this.stopCollecting();
      }

      this.setElementValue(defaultValue);

      this.invoke('html5ls:setDefaultValue:after', defaultValue, this.restoredValue);
      return defaultValue;
    },

    /**
     * Set restored value to the element.
     *
     * @returns {String}
     */
    setRestoredValue: function () {
      var restoredValue = this.restoredValue;
      var scope = this.invoke('html5ls:setRestoredValue:before', restoredValue);
      if (scope.restoredValue !== undefined) {
        restoredValue = scope.restoredValue;
      }

      this.showDefault = false;

      this.defaultValue = this.defaultValue
        ? this.defaultValue
        : this.getElementValue();

      if (restoredValue) {
        this.setElementValue(
          restoredValue != ''
            ? restoredValue
            : this.defaultValue
        );
      }

      this.invoke('html5ls:setRestoredValue:after', restoredValue, this.defaultValue);
      return restoredValue;
    },

    /**
     * Collect value from element and store it in local storage.
     */
    collectValue: function () {
      if (!this.collect) {
        return false;
      }

      var value = this.getElementValue();
      var data = this.getData();

      if (value != data.value && (value || data.value != undefined)) {
        var scope = this.invoke('html5ls:save:before', this.key, this.id, value);
        if (scope.value !== undefined) {
          value = scope.value;
        }
        this.setData(value);

        this.invoke('html5ls:save:after', this.key, this.id, value);
      }

      return true;
    },

    /**
     * Stop collecting and storing data.
     */
    stopCollecting: function () {
      this.collect = false;
    },

    /**
     * Start collecting and storing data.
     */
    startCollecting: function () {
      this.collect = true;
    },

    /**
     * Get restored element data from LocalStorage.
     *
     * @returns {*|string}
     */
    getData: function () {
      return this.Storage.getData(this.key, this.id);
    },

    /**
     * Save element value to the LocalStorage.
     *
     * @param value
     */
    setData: function (value) {
      this.Storage.setData(this.key, this.id, value, this.settings.expire);
    },

    /**
     * Get element value.
     *
     * @returns {*}
     */
    getElementValue: function () {
      // Allow others to interact with element value.
      var scope = this.invoke('html5ls:getElementValue', this.key, this.id);
      return scope.value;
    },

    /**
     * Set element value.
     *
     * @param value
     */
    setElementValue: function (value) {
      // Allow others to interact with element value.
      var scope = this.invoke('html5ls:setElementValue:before', this.key, this.id, value);
      if (scope.value !== undefined) {
        value = scope.value;
      }

      scope = this.invoke('html5ls:setElementValue', this.key, this.id, value);
      if (scope.value !== undefined) {
        value = scope.value;
      }

      this.invoke('html5ls:setElementValue:after', this.key, this.id, value);
    },

    /**
     * Get state of local storage element.
     *
     * Returns info about current state of element:
     *  true: default
     *  false: restored
     */
    getState: function () {
      return this.showDefault;
    },

    /**
     * Invoke a hook (trigger event).
     *
     * @param hook
     * @returns {{}}
     *  Returns scope variable that contains data added by other interactors.
     */
    invoke: function (hook) {
      var scope = {};
      var args = [scope];
      for (var i = 1; i < arguments.length; i++) {
        args.push(arguments[i]);
      }

      this.$el.trigger(hook, args);
      return scope;
    }
  };

  /**
   * jQuery plugin to help to work with LocalStorage class.
   *
   * @returns {LocalStorage}
   * @constructor
   */
  $.fn.html5ls = function (Storage, key, settings) {
    var html5lsElement = this.data('html5lsElement');
    if (!html5lsElement && key) {
      html5lsElement = new LocalStorage(this, Storage, key, settings);
      this.data('html5lsElement', html5lsElement);
    }

    return html5lsElement;
  };
})(jQuery);
