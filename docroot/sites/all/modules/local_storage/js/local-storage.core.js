/**
 * LocalStorageCore class.
 *
 * @param {String} key
 *    LocalStorage identifier. (E.g window.localStorage(key, data) )
 * @param {Object} settings
 *
 * @constructor
 */
function LocalStorageCore(key, settings) {
  this.storage = {};
  this.storageKey = key;
  this.settings = settings;
  this.init();
}

(function ($) {
  LocalStorageCore.prototype = {
    /**
     * @type jQuery
     */
    $elements: $(),
    /**
     * @type Object - Local Storage settings.
     */
    settings: {},
    /**
     * @type String - Key of the LocalStorage.
     */
    storageKey: '',
    /**
     * @type Object - Storage itself.
     */
    storage: {},

    /**
     * Init storage.
     */
    init: function () {
      var me = this;

      me.getStorage();

      if (me.settings.elements) {
        $.each(me.settings.elements, function (key, elements) {
          $.each(elements, function (id) {
            me.$elements = me.$elements.add($('#' + id));
          });
        });
      }

      if (me.settings.remove) {
        $.each(me.settings.remove, function (key, elements) {
          $.each(elements, function (id) {
            me.rmData(key, id);
          });
        });
      }

      me.run(300);
    },

    /**
     * Fetch and decode local storage data.
     *
     * @returns {*}
     */
    getStorage: function () {
      this.storage = localStorage.getItem(this.storageKey);
      try {
        this.storage = JSON.parse(this.storage);
        if (!this.storage) {
          this.storage = {};
        }
        for (var key in this.storage) {
          if (!this.storage.hasOwnProperty(key)) {
            continue;
          }

          for (var id in this.storage[key]) {
            if (!this.storage[key].hasOwnProperty(id)) {
              continue;
            }

            if (parseInt((new Date()).getTime() / 1000) >= this.storage[key][id].expire) {
              this.rmData(key, id);
            }
          }
        }
      } catch (e) {
        this.storage = {};
      }

      return this.storage;
    },

    /**
     * Get data from storage.
     *
     * @param key
     * @param id
     * @returns {*}
     */
    getData: function (key, id) {
      if (this.storage.hasOwnProperty(key)) {
        if (this.storage[key].hasOwnProperty(id)) {
          return this.storage[key][id];
        }
        else {
          return {};
        }
      }
      else {
        return {};
      }
    },

    /**
     * Set data in the storage.
     *
     * @param key
     * @param id
     * @param value
     * @param expire
     */
    setData: function (key, id, value, expire) {
      if (!this.storage.hasOwnProperty(key)) {
        this.storage[key] = {};
      }

      if (!this.storage[key].hasOwnProperty(id)) {
        this.storage[key][id] = {};
      }

      expire = expire || 48;
      expire *= 3600;

      this.storage[key][id].value = value;
      this.storage[key][id].expire = parseInt((new Date()).getTime() / 1000) + expire;
    },

    /**
     * Remove stored data from localStorage
     *
     * @param key
     * @param id
     * @returns {boolean}
     */
    rmData: function (key, id) {
      if (!this.storage.hasOwnProperty(key)) {
        return false;
      }

      if (!this.storage[key].hasOwnProperty(id)) {
        return false;
      }

      delete this.storage[key][id];

      var empty = true;
      for (var eachKey in this.storage[key]) {
        if (!this.storage[key].hasOwnProperty(eachKey)) {
          empty = false;
        }
      }

      if (empty) {
        delete this.storage[key];
      }

      this.save();

      return true;
    },

    /**
     * Encode and save storage to localStorage.
     *
     * @param [storage]
     */
    save: function (storage) {
      try {
        storage = storage || this.storage;
        localStorage.setItem(this.storageKey, JSON.stringify(storage));
      } catch (e) {
        alert(Drupal.t('Your local storage is full: @err', {'@err': e}));
      }
    },

    /**
     * Start collecting values.
     *
     * @param {Number} interval
     */
    run: function (interval) {
      var me = this;
      me.runHandler = setInterval(function () {
        me.$elements.trigger('html5ls:collect');
        me.save();
      }, interval);
    },
    /**
     * Stop collecting values.
     *
     * @returns {boolean}
     */
    stop: function () {
      if (this.runHandler) {
        clearInterval(this.runHandler);
        return true;
      }
      return false;
    },

    /**
     * Clear all values in the localStorage.
     */
    clearAll: function () {
      this.storage = {};
      this.save();
    }
  };
})(jQuery);
