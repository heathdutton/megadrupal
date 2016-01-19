/**
 * @file
 *	Drupal State module
 */

/**
 * @constructor
 * @class
 * @singleton
 * @param {obj} $
 *	- jQuery
 */
var State_Admin = function($) {
	var self = this,
  _localEnOverrides = {},
  _passValue_enOverride, _checkRole, _disableRoles;
  /**
   * Pass value of a form field to object list of all (relevant) form fields, and update JSON object in aggregate form field.
   *
   * Updates aggregate value to empty string, if all fields are empty.
   *
   * @return {void}
   */
  _passValue_enOverride = function() {
    var k, nonEmpty;
    _localEnOverrides[ this.getAttribute("name").replace(/local_en__/, "") ] = $.trim(this.value);
    for(k in _localEnOverrides) {
      if(_localEnOverrides.hasOwnProperty(k) && _localEnOverrides[k].length) {
        nonEmpty = true;
        break;
      }
    }
    $("textarea[name='state__enovrrd']").get(0).value = nonEmpty ? JSON.stringify(_localEnOverrides) : "";
  };
  /**
   * Make sure the same role isnt selected for both modes.
   *
   * @return void
   */
  _checkRole = function() {
    var keys;
    if(this.checked) {
      keys = this.getAttribute("name").replace(/^state__(prlngu?n?cnd)\[([^\]]+)\]$/, "$1,$2").split(/,/);
      if(keys[0] === "prlngcnd") {
        if($("input[name='state__prlnguncnd[" + keys[1] + "]']").get(0).checked) {
          alert(
              Drupal.t("You cannot select the same role!nlfor conditional prolongation as well as!nlunconditional prolongation").
                replace(/\!nl/g, "\n")
          );
          this.checked = false;
        }
      }
      else if($("input[name='state__prlngcnd[" + keys[1] + "]']").get(0).checked) {
        alert(
            Drupal.t("You cannot select the same role!nlfor unconditional prolongation as well as!nlconditional prolongation").
              replace(/\!nl/g, "\n")
        );
        this.checked = false;
      }
    }
  };
  /**
   * Make authenticated user role disable all other roles (but anonymous user).
   *
   * @return void
   */
  _disableRoles = function() {
    var nm = this.getAttribute("name") === "state__prlngcnd[-1]" ? "prlngcnd" : "prlnguncnd", disable = this.checked ? "disabled" : false, nm1;
    $("div#edit-state-" + nm + " input[type='checkbox']").each(function() {
      if((nm1 = this.getAttribute("name")) !== "state__" + nm + "[-2]" && nm1 !== "state__" + nm + "[-1]") {
        if(disable) {
          this.checked = false;
        }
        this.disabled = disable;
      }
    });
  };
  /**
   * Unpack aggregate values, and set event handlers on all relevant form fields.
   *
   * @return {void}
   */
  this.init = function() {
    var jq, v, k, r, hasEnOverride;

    //  This module cannot be configured without JSON support (IE<8).
    if(!window.JSON || typeof JSON.stringify !== "function" || typeof JSON.parse !== "function") {
      alert(
          "This module cannot be configured using current browser\nbecause the browser has no JSON support." +
          ($.browser.msie ? "\n\nInternet Explorer <8 never supports JSON, version 8 may support it, version >8 always supports it." : "")
      );
      return;
    }

    //  Make frontend session management checkbox control expand/collapse of child fieldset.
    if((jq = $("input#edit-state-frntnd")).get(0).checked) {
      $("fieldset#edit-frontend-session-configure").removeClass("collapsed");
    }
    jq.click(function(){
      $("fieldset#edit-frontend-session-configure")[ (this.checked ? "remove" : "add") + "Class" ]("collapsed");
    });

    //  Prevent checking the same role for conditional and unconditional modes.
    $("div.prolong-roles input.form-checkbox").change(_checkRole);
    //  Make authenticated user role disable all other roles (but anonymous user).
    if((r = $("input[name='state__prlngcnd[-1]']").change(_disableRoles).get(0)).checked) {
      _disableRoles.apply(r, []);
    }
    if((r = $("input[name='state__prlnguncnd[-1]']").change(_disableRoles).get(0)).checked) {
      _disableRoles.apply(r, []);
    }

    //  Combine English message override fields into a single hidden textarea containing JSON object.
    if((v = $("textarea[name='state__enovrrd']").get(0).value)) {
      _localEnOverrides = JSON.parse(v);
      for(k in _localEnOverrides) {
        if(_localEnOverrides.hasOwnProperty(k)) {
          hasEnOverride = true;
          $("input[name='local_en__" + k + "']").get(0).value = _localEnOverrides[k];
        }
      }
    }
    if(hasEnOverride) {
      $("fieldset#edit-override-english").removeClass("collapsed");
    }
    $("input.local-en-override").each(function() {
      $(this).change(_passValue_enOverride);
    });
  };

};
//	instantiate and run init at window onload
(function($) {
	if(!$) {
		return;
	}
	$(document).bind("ready", function() {
		(State_Admin = new State_Admin($)).init();
	} );
})(jQuery);
