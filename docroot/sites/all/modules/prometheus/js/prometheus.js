/** Prometheus JavaScript hooks for AJAX html fragment support */

(function ($) {
  // Check for history support
  if ( typeof ( History ) == "undefined" )
  {
    History = { enabled: false };
  }

  //
  var CURRENT_HISTORY_STATE = 0;

  var CALLBACKS = {
    FragmentsArrived: [],
    FragmentsError:   []
  }

  function prometheus_serialize(form) {
    var data = {
      _PROMETHEUS_AJAX: true
    };

    $("input, textarea, select, button", form).each(function(){
      if ( this.type == "radio" && this.checked )
      {
        data[this.name] = this.value;
      }
      else if ( this.type == "checkbox" && this.checked )
      {
        data[this.name] = "on"
      }
      else
      {
        data[this.name] = $(this).val();
      }
    });

    return data;
  }

  /** Triggers all registered callbacks in the supplied callback stash */
  function triggerCallbacks(callback_stash) {
    var args = $.merge([], arguments);
    args.splice(0,1);

    for (var index in callback_stash)
    {
      var callback = callback_stash[index];
      try {
        callback.fn.apply(callback.context, args);
      } catch (exceptions){} // and ignore them
    }
  }

  function prometheus_response_callback(triggerElt, url, registerState) {
    return function(jqXHR, status) {
      // handle errors
      switch (status) {
        case "error":
        case "timeout":
        case "abort":
          triggerCallbacks(CALLBACKS.FragmentsError, jqXHR.triggerElt, jqXHR)
          return;
      }

      // parse JSON data
      var data = eval ("(" + jqXHR.responseText + ")");

      // empty all the regions
      $("[region]").html("");

      // itterate trough all returned regions and populate containers on page
      for (var destName in data) {
        // ignore prometheus special variables
        if ( destName.indexOf("_PROMETHEUS_") == 0 ) continue;

        $dest = $("#" + destName );
        $dest
          .html(data[destName])
          .hide()
          .fadeIn();

        // process any new forms we got back
        prometheus_hook_elements($dest);
      }

      // hide any remaining empty regions
      prometheus_hide_empty_regions();

      if (History.enabled && registerState) {
        // update the history state
        CURRENT_HISTORY_STATE++;
        var state = {
          id: CURRENT_HISTORY_STATE
        };
        History.pushState(state, data._PROMETHEUS_TITLE || document.title, url);
      }

      // trigger any user callbacks
      triggerCallbacks(CALLBACKS.FragmentsArrived, triggerElt, jqXHR);
    }
  }

  function prometheus_submit_form() {
    var opts = {
        url:  this.action
      , type: this.method.toUpperCase() || "POST"
      , data: prometheus_serialize(this)
      , dataType: "text"
      , complete: prometheus_response_callback(this, this.action)
    };

    var xhr = $.ajax(opts); // .ajax

    // prevent default handler from occuring
    return false;
  }

  function prometheus_click_link() {
    var opts = {
        url:  this.href
      , type: "POST"
      , data: { _PROMETHEUS_AJAX: true }
      , dataType: "text"
      , complete: prometheus_response_callback(this, this.href, true)
    };

    var xhr = $.ajax(opts);

    // prevent default handler from occuring
    return false;
  }

  function prometheus_hook_elements(dest) {
    $("form, a", dest).each(function() {
      // hook only into elements that have ajax="true" attribute
      if ( ! this.getAttribute("ajax") ) return;

      switch (this.nodeName.toUpperCase()) {
        case "FORM":
          // override form submit
          $(this).submit(prometheus_submit_form);
          break;
        case "A":
          if (History.enabled) {
            $(this).click(prometheus_click_link);
          }
          break;
      }
    });
  }

  function prometheus_hide_empty_regions() {
    $("[region]").each(function() {
      var $region = $(this);
      if ($region.html() == "") {
        $region.hide();
      }
    });
  }

  var Prometheus = {
    addFragmentsArrivedCallback: function(callback, context) {
      CALLBACKS.FragmentsArrived.push({
        fn:       callback,
        context:  context
      });
    },
    addFragmentsErrorCallback: function(callback, context) {
      CALLBACKS.FragmentsError.push({
        fn:       callback,
        context:  context
      });
    }
  }

  // export Prometheus
  window.Prometheus = Prometheus;

    // init on dom load
    $(function() {
      prometheus_hook_elements();
      prometheus_hide_empty_regions();
    });

  if (History.enabled) {
    // Bind to StateChange Event
    History.Adapter.bind(window,'statechange',function(){
      var state = History.getState();
      var state_id = state.data.id;
      // ensure we aren't being notified about the state we just pushed
      if (state_id == CURRENT_HISTORY_STATE) return;
      CURRENT_HISTORY_STATE = state_id;
      // force browser to go back to the server to load the previous state
      location.href = state.url;
    });
  }

})(jQuery);
