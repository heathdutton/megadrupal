/**
 * @file
 *  Drupal State module
 */

'use strict';
/*jslint browser: true, continue: true, indent: 2, newcap: true, nomen: true, plusplus: true, regexp: true, white: true*/
/*global alert: false, confirm: false, console: false*/
/*global jQuery: false, Drupal: false*/

(function($) {

/**
 * Checks that session is alive and current, and optionally prolongs session by making AJAX requests.
 * Singleton, instantiated to itself.
 * @constructor
 * @name State
 * @singleton
 */
var State = function($) {
  /**
   * @ignore
   * @private
   * @type {State}
   */
  var self = this,
  /**
   * @ignore
   * @private
   * @type {boolean|undefined}
   */
  _init,
  /**
   * @ignore
   * @private
   * @type {object}
   */
  _ = {
    //  {string}
    cookieDomain: "",
    //  {string}
    cookiePath: "/",
    //  {bool}
    cookieSecure: false,
    //  {string}
    cookieNamespace: "",
    //  (integer|undefined)
    pageRespNo: undefined,
    //  {integer}
    prolong: 0,
    //  {integer}
    checkFirst: 60 * 1000, // Dont call in right away (may be bouncing page view), and dont wait too long (prolongs session).
    //  {integer}
    checkInterval: 0,
    //  {Date|null}
    lastCheckTime: null,
    //  {integer}
    sessWarnTimeout: 0,
    //  {integer}
    sessTimeout: 0,
    //  {Date|null}
    sessExpireTime: null,
    //  {Date|null}
    sessWarnTime: null,
    //  {boolean}
    sessCurrent: true,
    //  {boolean}
    sessExpired: false,
    //  {Date|null}
    sessProlongedTime: null,
    //  {boolean}
    activitySensed: false,
    //  {boolean}
    revive: false,
    //  {Date|null}
    formExpireTime: null,
    //  {Date|null}
    formWarnTime: null,
    //  {boolean}
    formExpired: false,
    //  {boolean}
    autoRedirect: false,
    //  {string}
    redirectTarget: "",
    //  (string|undefined)
    sid: undefined,
    //  {array}
    errors: [],
    //  {string}
    infoCurrent: "",
    //  {integer}
    infoLife: 60000
  },
  /**
   * @private
   * @type {boolean|undefined}
   */
  _jqUiDial,
  /**
   * @private
   * @type {object|undefined}
   */
  _localEn,
  //  Declare private methods, to make IDEs list them
  _errorHandler, _local, _o, _check, _ajax, _ajaxCompleted, _url, _redirect, _info;

  /**
   * Error handler, give it an error or a variable.
   *
   * Does nothing if no Inspect module, or if Inspect's 'Enable frontend javascript variable/trace inspector' permission is missing for current user.
   *
   * @see inspect.errorHandler
   * @ignore
   * @private
   * @param {Error} [error]
   * @param {*} [variable]
   * @param {object|integer|boolean|string} [options]
   * @return {void}
   */
  _errorHandler = function(error, variable, options) {
    var u = options, o = {}, t;
    //  Do nothing, if inspect is the 'no action' type.
    if(typeof window.inspect === "function" && inspect.tcepsni) {
      if(typeof inspect.errorHandler === "function") {
        if(u) {
          if((t = typeof u) === "string") {
            o.message = u;
          }
          else if(t === "object") {
            o = u;
          }
          //  Otherwise: ignore; use object argument for options if other properties are needed.
        }
        o.category = "State";
        inspect.errorHandler(error, variable, o);
      }
      else {
        inspect.console("Please update Inspect.");
      }
    }
  };
  /**
   * @ignore
   * @private
   * @param {string} nm
   * @return {string}
   */
  _local = function(nm) {
    var s;
    //  S.... Drupal.t() doesnt use the 'g' flag when replace()'ing, so Drupal.t() replacement is utterly useless - and nowhere to report the bug :-(
    if(!(s = _o(_localEn, nm))) { // English t message overridden?
      switch(nm) {
        case "preExpire":
          s = Drupal.t("This page is about to expire!newline!newlinePlease save now, if you have made any changes.!newlineOtherwise, please !link_startreload the page!link_end.");
          break;
        case "expired_redirect":
          s = Drupal.t("This page has expired!newline!newlinePlease !link_startproceed to the start page!link_end.");
          break;
        case "expired_reload":
          s = Drupal.t("This page has expired!newline!newlinePlease !link_startreload the page!link_end.");
          break;
        default:
          s = "[LOCAL: " + nm + "]";
      }
    }
    return s.replace(/\!newline/g, "\n");
  };
  /**
   * Object/function property getter, Object.hasOwnproperty() alternative.
   *
   * @ignore
   * @param {object} o
   * @param {string|integer} k0
   * @param {string|integer} [k1]
   * @return {*}
   *  - undefined: o not object, or o doesnt have property k0, or the value of o[k1] is undefined; and the same goes if arg k1 is used
   */
  _o = function(o, k0, k1) {
    var t = typeof o;
    return o && (t === "object" || t === "function") && o.hasOwnProperty(k0) ?
      (k1 === undefined ? o[k0] : (
        (o = o[k0]) && ((t = typeof o) === "object" || t === "function") && o.hasOwnProperty(k1) ? o[k1] : undefined
      ) ) : undefined;
  };
  /**
   * @ignore
   * @private
   * @param {Event|falsy} evt
   * @param {string} action
   * @return void
   */
  _check = function(evt, action) {
    var u, u1, d, to, act = action, ci, f;
    if(!_.sessCurrent && !_.sessExpired && !_.formExpired) { // Sanity check.
      return;
    }
    d = _.lastCheckTime = new Date();
    if(!(_.sessCurrent = (_.sid === self.cookieGet("state__sid")))) {
      _.sessCurrent = false;
      _info("expired");
      if(_.autoRedirect) {
        _redirect(_url(), _.infoLife);
      }
      //  Get out.
      return;
    }
    if(action === "getTimeout") {
      //  Set visitor activity listeners, if conditional prolongation.
      if(_.prolong === 2) {
        $(document.documentElement).bind("click mousemove keyup", f = function() {
          _.activitySensed = true;
          if(!_.revive) {
            return;
          }
          _.revive = false;
          if(new Date() < _.formWarnTime) { // The other basic conditions are evaluated in the beginning of _check().
            _check(null, "check");
          }
        });
        $(window).scroll(f);
      }
      //  Do ajax request (fall thru).
    }
    //  We will never get here if the getTimeout request fails, because only successful getTimeout response starts first check timer.
    else if(action === "check") {
      //  Update session expire time, if this or another window/tab has made prolonging request recently.
      if((u = self.cookieGet("state__ssprlngd")) && u) {
        u = u.split(",");
        if(u.length === 2 && u[1] === _.sid && /^\d{13,14}$/.test(u[0]) &&
            (u = parseInt(u[0], 10)) && (u1 = new Date(u)) &&
            u1 > _.sessProlongedTime) {
          _.sessProlongedTime = u1;
          _.sessExpireTime = new Date( u + _.sessTimeout );
          _.sessWarnTime = new Date( u + _.sessWarnTimeout ); // Warn visitor 10% earlier than timeout.
        }
      }
      ci = _.checkInterval;
      if(d >= _.sessExpireTime) { // Destroy session, if expired.
        _.sessExpired = true;
        _info("expired");
        //  Help session garbage collection; destroy the session via backend.
        //  But wait, some other window/tab might be doing some work, which should be allowed to finish before session is destroyed
        to = setTimeout(function() {
          if(_.sid === self.cookieGet("state__sid")) {
            _ajax("destroySession");
          }
        }, ci = Math.floor(ci / 2));
        if(_.autoRedirect) {
          _redirect(_url(), ci + _.infoLife); // One minute to let destroySession call sink in.
        }
        //  Get out.
        return;
      }
      else if((u = _.formExpireTime) && d >= u) { // Dont prolong session, if form expired.
        _.formExpired = true;
        _info("expired");
        if(_.autoRedirect) {
          _redirect(_url(), _.infoLife);
        }
        return;
      }
      //  Tell visitor if form about to expire (and not telling that at the moment).
      else if((u = _.formWarnTime) && d >= u && _.infoCurrent !== "preExpire_form") {
        _info("preExpire_form");
      }
      //  Prolong session?
      act = null;
      switch(_.prolong) {
        case 3: // Unconditional prolongation
          act = "prolong"; // Continue to ajax request
          break;
        case 2: // User activity based prolongation
          if(_.activitySensed) {
            _.activitySensed = false; // Clear for next interval.
            //  If was closing in on session expiration, clear such info.
            if(_.infoCurrent === "preExpire_sess") {
              _info(); // Hide, and clear _.infoCurrent.
            }
            act = "prolong"; // Continue to ajax request
          }
          break;
        // default: 0|1 ~ no prolongation
      }
      //  Regulate check timeout.
      //  Skip prolongation and modify timeout if this or another browser tab recently prolonged session (~ obsolete requests when multiple tabs).
      if((u = _.sessProlongedTime) &&
          //  The 0.8 multiplier is part of fuzzy logic algo, which ensures approximately correct interval, even if more than one browser tab.
          u.getTime() > d.getTime() - (ci * 0.8)) {
        act = "skip"; // Do setTimeout, but skip AJAX call.
        //  Random number between 60% and full checkInterval (part of fuzzy logic algo).
        ci = Math.floor((ci * 0.6) + ( ( Math.random() * ( (ci * 0.4) + 1 ) ) + 1 ) - 1);
      }
      //  If closing in to session or form expire - risk of timeout before next check - make next checkInterval smaller (half).
      if((u = new Date(d.getTime() + ci)) > _.sessExpireTime || u > _.formExpireTime) {
        ci = Math.floor(ci / 2);
      }
      //  Start new check timer
      to = setTimeout(function() {
        _check(null, "check");
      }, ci);
      if (!act) { // No prolongation at all, or no user activity sensed (not relevant when unconditional prolongation, because then no session expire).
        //  Tell visitor if session about to expire (and not telling anything at the moment).
        if(!_.infoCurrent && d >= _.sessWarnTime) {
          _info("preExpire_sess");
          if(_.prolong === 2) { // If conditional prolongation, allow user activity to check/prolong session and hide preExpire_sess warning.
            _.revive = true;
          }
        }
        return;
      }
    }
    //  Ping server to keep session alive (or, first time, retrieve session timeout).
    if(act === "prolong" || act === "getTimeout") { // And not 'skip'.
      _ajax(act);
    }
  };
  /**
   * @ignore
   * @private
   * @param {string} act
   * @return {void}
   */
  _ajax = function(act) {
    $.ajax({
      url: "/state/ajax/" + _.pageRespNo,
      type: "POST",
      data: {
        action: act,
        arg: act !== "destroySession" ? null : _.sid
      },
      dataType: "json", // expects json formatted response data
      cache: false,
      /**
        * @return {void}
        * @param {object} responseData
        *  - (string) action
        *  - (boolean) success
        *  - (string) error
        *  - (string) message
        *  - (integer|undefined) sessionTimeout
        * @param {string} textStatus
        * @param {object} jqXHR
        */
      success: function(oResp, textStatus, jqXHR) {
        var u, d, t, st, sw, ci, to, to1;
        if(textStatus === "success" && $.type(oResp) === "object") {
          if(oResp.success) {
            if(oResp.action === "destroySession" ||
              !_.sessCurrent || _.sessExpired
            ) {
              return;
            }
            if(_.sid !== self.cookieGet("state__sid")) {
              _.sessCurrent = false;
              _info("expired");
              if(_.autoRedirect) {
                _redirect(url(), _.infoLife);
              }
              return;
            }
            t = (d = new Date()).getTime();
            switch(oResp.action) {
              case "prolong":
                _.sessExpireTime = new Date( t + _.sessTimeout );
                _.sessWarnTime = new Date( t + _.sessWarnTimeout );
                break;
              case "getTimeout":
                if((st = oResp.sessionTimeout)) {
                  st *= 1000; // Server sessionTimeout is in seconds, not milliseconds.
                  //  Modify checkInterval if more than (effective) 20% of session timeout.
                  if((ci = _.checkInterval) > st / 6) { // 16.7% because we subtract checkInterval from sessTimeout.
                    _.checkInterval = ci = Math.floor(st / 6);
                  }
                  //  Subtract checkInterval from server session timeout, otherwise a window/tab may face destroyed session unknowingly.
                  _.sessTimeout = st = st - ci;
                  _.sessExpireTime = new Date( t + st );
                  //  Warn visitor 10% earlier than timeout
                  //  But the time between warning and timeout must be at least checkInterval;
                  //  to raise the probability that the session warning actually gets displayed.
                  if((sw = Math.floor(st / 10)) < ci) {
                    sw = ci;
                  }
                  _.sessWarnTime = new Date( t + (_.sessWarnTimeout = st - sw) );
                  //  Start timer.
                  to = setTimeout(function(){
                    _check(null, "check");
                  }, ci);
                  //	Register AJAX complete listener.
                  to1 = setTimeout(function(){
                    $("body").ajaxComplete(_ajaxCompleted);
                  }, 1000);
                }
                break;
              default: // unknown action
                try {
                  throw new Error("Unknown action["+oResp.action+"]")
                }
                catch(er) {
                  _errorHandler(er, null, "State._check()");
                }
                return;
            }
            _.sessProlongedTime = d;
            self.cookieSet("state__ssprlngd", "" + t + "," + _.sid); // set last time session prolonged cookie
          }
          else if(oResp.error === "no_session") {
            _.sessCurrent = false;
          }
          else {
            _errorHandler(oResp, null, "State._check()");
          }
        }
        else {
          _.errors.push("response: " + textStatus);
          _errorHandler(null, {
            textStatus: textStatus,
            response_data: oResp
          }, "State._check()");
        }
      },
      error: function(jqXHR, textStatus, errorThrown) {
        if(jqXHR && jqXHR.status === 403) {
          _.sessCurrent = false;
        }
        else {
          _.errors.push("response: " + textStatus);
          _errorHandler(null, {
            textStatus: textStatus,
            errorThrown: errorThrown
          }, "State._check()");
        }
      }
    });
  };
  /**
   * jQuery AJAX handler, which tells State that the current session got prolonged now, due to response of a jQuery AJAX request.
   *
   * @ignore
   * @private
   * @param {Event} e
   * @param {XMLHttpRequest} xhr
   * @param {object} settings
   * @return {void}
   */
  _ajaxCompleted = function(e, xhr, settings) {
    var u;
    //if (_o(xhr, "status") === 200 &&
    if (xhr.status === 200 && // IE<9 XMLHttpRequest object has no hasOwnProperty method
      typeof (u = _o(settings, "url")) === "string" && u.indexOf("/") === 0 &&
      u.indexOf("/state/ajax/") !== 0
    ) { // State's own responses are handled in _check
      self.sessionProlonged();
    }
  };
  /**
   * Creates url to either current url or redirectTarget.
   *
   * Removes trailing hash (reload; current url only), because otherwise links/redirects wont work.
   * Drupal's login system removes the hash anyway, when redirecting to 403'ed url after new login.
   *
   * @ignore
   * @private
   * @param {boolean} [reload]
   *  - default: false (~ uses redirectTarget setting)
   *  - truthy: uses current url
   * @return {string}
   *  - url to current url or redirectTarget
   */
  _url = function(reload) {
    var url, loc = window.location, v;
    if(reload || !(url = _.redirectTarget)) { // ~ Empty url.
      return loc.href.replace(/#.*$/, "");
    }
    return url.charAt(0) === "/" ?
      (loc.protocol + "//" + loc.hostname + (!(v = loc.port) ? "" : (":" + v)) + url) :
      url; // http://...
  };
  /**
   * Attempts redirect, delayed.
   *
   * @ignore
   * @private
   * @param {string} url
   * @param {integer} [delay]
   *  - default: 20 milliseconds
   * @return {void}
   */
  _redirect = function(url, delay) {
    var to = setTimeout(function(){
      window.location.href = url;
    }, delay || 20)
  };
  /**
   * Tell visitor something.
   *
   * @ignore
   * @private
   * @param {string|falsy} type
   *  - if falsy, clears/hides info
   *  - string: preExpire_form|preExpire_sess|expired
   * @return {void}
   */
  _info = function(type) {
    var ms, url, a, ttl, dial, to;
    _.infoCurrent = type || "";
    if(type) {
      if(type === "expired") {
        url = _url();
        ms = !_.redirectTarget ? "expired_reload" : "expired_redirect";
      }
      else {
        url = _url(true); // Always reload to current page, when pre-expire warning.
        ms = "preExpire";
      }
      ms = _local(ms).replace(/\!link_start/, "<a href=\"" + (url || "") + "\">").replace(/\!link_end/, "</a>");
    }
    if(_jqUiDial) {
      if(type) {
        a = ms.split(/\n\n/);
        ttl = a[0].replace(/\n/g, "").replace(/\.$/, "");
        a.splice(0, 1);
        ms = '<p>' + a.join("</p><p>").replace(/\n/g, "<br/>") + "</p>";
      }
      if(!(dial = document.getElementById("state_info"))) {
        if(!type) {
          return;
        }
        //  create dialog ----------------------------------------
        if(!document.getElementById("state_info_container")) {
          $(document.body).append("<div id=\"state_info\">" + ms + "</div>");
        }
        dial = document.getElementById("state_info");
        //  instantiate dialog -----------------------------------
        $(dial).dialog({
          stack: true,
          modal: true,
          autoOpen: false,
          closeOnEscape: type !== "expired", // Prohibit close on escape if expired, force user to press button.
          resizable: false,
          position: ["center","center"],
          title : ttl,
          buttons: [
            {
              text: Drupal.t("OK"),
              click: type !== "expired" ? function(){$(this).dialog("close");} : function(){} // We set furtherdown.
            }
          ],
          beforeClose: function() {
            _.infoCurrent = "";
          }
        });
        //  Add our class on dialog's outer container, and make if position:fixed.
        $(dial.parentNode).css("position", "fixed").addClass("module-state-dialog-container");
      }
      else if(!type) {
        $(dial).dialog("close");
        return;
      }
      else {
        $(dial).dialog("option", "title", ttl);
        $(dial).html(ms);
      }
      if(type === "expired") {
        $("a.ui-dialog-titlebar-close", "div.module-state-dialog-container").unbind().hide();
        $(dial).dialog("option", "buttons", [
          {
            text: Drupal.t("OK"),
            click: function(){_redirect(_url());}
          }
        ]);
      }
      //  Show the dialog; delayed, otherwise modal overlay wont cover the whole window.
      to = setTimeout(function(){
        $(dial).dialog("open");
      }, 5000); // Safe-guarding against very slow rendering.
    }
    //  Using alert() may not work well, because alert() may block timers (and it does prevent redirect in IE):
    else if(type) {
      alert(ms.replace(/\!link_[a-z]+/g, "")); // !link_start, !link_end
    }
    //  Cant close alert() programmatically; only user can do that.
  };

  /**
   * Setup cookie features etc., and session checking/prolongation (if any).
   *
   *  Options:
   *  - (string) sid ~ hash of session id (at page load)
   *  - (integer) formExpire ~ form expiration (in seconds), or zero (forms never expire)
   *  - (integer) prolong (default 0), 0 ~ none | 1 ~ only checking | 2 ~ visitor activity conditioned | 3 ~ unconditional
   *  - (integer) checkInterval (default 300), in seconds
   *  - (boolean|integer) autoRedirect (default false ~ redirect when user closes info dialog)
   *  - (string) redirectTarget (default empty ~ reload, redirect to current page)
   *  - (object) localEn (default empty)
   * @function
   * @name State.init
   * @param {string} cookieDomain
   * @param {string} cookiePath
   * @param {string} cookieNamespace
   * @param {integer} pageRespNo
   *  - page response number ~ how many times backend State has positively detected that a (any) page was delivered
   * @param {object|falsy} [options]
   * @return {void}
   */
  this.init = function(cookieDomain, cookiePath, cookieNamespace, pageRespNo, options) {
    var o = options, u, v, t = new Date().getTime(), fx;
    if(_init) {
      return;
    }
    _init = true;
    _.cookieDomain = cookieDomain;
    _.cookiePath = cookiePath; // Or: Drupal.settings.basePath. But that may be removed in later version of core (and why isnt cookie domain there too?).
    _.cookieNamespace = cookieNamespace;
    _.pageRespNo = pageRespNo;
    if(!o || typeof o !== "object" ||
      !(_.sid = _o(o, "sid")) || // Hash of session id (at page load).
      !(_.formExpireTime = (fx = _o(o, "formExpire")) ? new Date(t + (fx * 1000)) : null)
    ) {
      return;
    }
    _.prolong = (u = _o(o, "prolong")) || 0;
    _.checkInterval = ((u = _o(o, "checkInterval")) ? u : 300) * 1000;
    _.formWarnTime = fx ? new Date(t + (fx * 900)) : null; // Warn visitor 10% earlier than expiration.
    _.autoRedirect = _o(o, "autoRedirect") ? true : false;
    _.redirectTarget = (u = _o(o, "redirectTarget")) ? u : "";
    _localEn = (u = _o(o, "localEn")) ? u : {};
    _.cookieSecure = window.location.protocol === "https:";
    _jqUiDial = typeof _o($, "ui", "dialog") === "function";
    //  Start timer
    if(_.prolong) {
      t = setTimeout(function() {
        _check(null, "getTimeout");
      }, _.checkFirst = (u = _.checkFirst) < (v = _.checkInterval) ? u : v); // First interval should not be larger than general interval.
    }
  };

  /**
   * Get list of domain-safe cookie defaults.
   *
   * Buckets:
   * - (string) path; '/' or '/path-without-traling-slash'
   * - (string) domain; '.domain'
   * - (boolean) secure; whether current location of current window is http|https
   * - (string) namespace; hash of http|https + domain + path
   *
   * @name State.cookieDefaults
   * @return {object}
   */
  this.cookieDefaults = function() {
    return {
      path: _.cookiePath,
      domain: _.cookieDomain,
      secure: _.cookieSecure,
      namespace: _.cookieNamespace
    };
  };
  /**
   * Set domain-safe cookie.
   *
   * @name State.cookieSet
   * @uses jQuery.cookie()
   * @param {string} name
   * @param {*} value
   *  - will be stringed
   *  - default: empty string
   * @param {object|falsy} [options]
   *  - (integer|Date) expire: default session (falsy), non-session must be set as a number of days or Date object
   *  - (string) path: default base_path, non-empty value is interpretated as relative to base_path
   * @return {void}
   */
  this.cookieSet = function(name, value, options) {
    var u = options, v, t, le, n, o = {
      path: _.cookiePath,
      domain: _.cookieDomain,
      secure: _.cookieSecure
    };
    if(u) {
      if((v = _o(u, "expire")) && ((t = typeof v) === "date" || (t === "number" && v > 0))) {
        o.expire = v;
      }
      if((v = _o(u, "path")) && (v = $(trim(v))) !== "/" && (le = v.length)) {
        if(v.charCodeAt(le -1) === "/") { // Remove trailing slash.
          v = v.substr(0, le -1);
          --le;
        }
        if(o.path === "/") {
          if(v.charCodeAt(0) === "/") {
            v = v.substr(1);
          }
        }
        else if(v.charCodeAt(0) !== "/") { // Default path isnt "/".
          v = "/" + v;
        }
        o.path += v;
      }
    }
    $.cookie(
      name + _.cookieNamespace,
      !(n = value) && n !== 0 ? "" : n, // Versus jQuery.cookie() doesnt set value if undefined|null.
      o
    );
  };
  /**
   * Increase value of domain-safe cookie.
   *
   * Logs trace if arg increment or the cookie value isnt falsy, number or number-like string.
   *
   * @name State.cookieIncrease
   * @uses jQuery.cookie()
   * @param {string} name
   * @param {number|string} increment
   *  - default: one
   * @param {object|falsy} [options]
   *  - (integer|Date) expire: default session (falsy), non-session must be set as a number of days or Date object
   *  - (string) path: default base_path, non-empty value is interpretated as relative to base_path
   * @return {number|boolean}
   *  - number: the new value of the cookie
   *  - false: arg increment or cookie value isnt falsy, number or number-like string
   */
  this.cookieIncrease = function(name, increment, options) {
    var u = increment || 1, v = $.cookie(name + _.cookieNamespace) || 0, a = [u, v], i, n, t, em;
    try {
      for(i = 0; i < 2; i++) {
        if((n = !i ? u : v)) {
          em = !i ? "arg increment" : "cookie value";
          if((t = typeof n) === "string") {
            if(n.indexOf(".") > -1) {
              if(!isFinite(a[i] = parseFloat(n))) {
                throw new Error(em + "[" + n + "] isnt float-like");
              }
            }
            else if(!isFinite(a[i] = parseInt(n, 10))) {
              throw new Error(em + "[" + n + "] isnt integer-like");
            }
          }
          else if(t !== "number") {
            throw new Error(em + "[" + n + "] isnt number|string|falsy");
          }
        }
      }
      self.cookieSet(name, a[1] += a[0], options);
      return a[1];
    }
    catch(er) {
      em = "State.cookieIncrease()";
      _errorHandler(er, null, em);
    }
    return false;
  };
  /**
   * Get domain-safe cookie.
   *
   * @name State.cookieGet
   * @uses jQuery.cookie()
   * @param {string} name
   * @return {string|null}
   *  - null: not set
   */
  this.cookieGet = function(name) {
    return $.cookie(name + _.cookieNamespace);
  };
  /**
   * Remove domain-safe cookie.
   *
   * Despite using jQuery.cookie the method doesnt rely on the related jQuery.removeCookie() method, because that method may not exist.
   *
   * @name State.cookieRemove
   * @uses jQuery.cookie()
   * @param {string} name
   * @param {object|falsy} [options]
   *  - (string) path: default base_path, non-empty value is interpretated as relative to base_path
   * @return {string|null}
   *  - null: not set
   */
  this.cookieRemove = function(name, options) {
    var o = options || {}, v;
    o.expire = new Date(1); // 1970.
    v = self.cookieGet(name);
    self.cookieSet(name, "", o);
    return v;
  };
  /**
   * Check whether session seems alive and current, and form hasnt expired.
   *
   * @function
   * @name State.alive
   * @return {boolean|undefined}
   *  - undefined: session checking/prolongation not set up
   */
  this.alive = function() {
    return !_.sid ? undefined : (
      (!_.sessCurrent || _.sessExpired || _.formExpired) ? false :
        (_.sessCurrent = (_.sid === self.cookieGet("state__sid")))
    );
  };
  /**
   * Page load number; how many times backend State has positively detected that a (any) page was delivered.
   *
   * @function
   * @name State.pageResponseNumber
   * @return {integer|undefined}
   *  - undefined: State not initialized, or no session checking/prolongation
   */
  this.pageResponseNumber = function() {
    return _.pageRespNo;
  };
  /**
   * Tell State that the current session got prolonged now, due to a (non jQuery) AJAX or iframe request.
   *
   * There is no need to call this upon response of a jQuery AJAX request; State registers all jQuery AJAX responses.
   * @function
   * @name State.sessionProlonged
   * @param {Date|falsy} [date]
   *  - default: now
   * @return {boolean}
   *  - false: session or form has expired, or the session related to current page load is not current anymore, or State not initialized
   */
  this.sessionProlonged = function(date) {
    if(this.alive()) {
      self.cookieSet("state__ssprlngd", "" + (_.sessProlongedTime = date || new Date()).getTime() + "," + _.sid);
      return true;
    }
    return false;
  };
  /**
   * Inspect current state.
   *
   * Does nothing if no Inspect module (or no-action version of Inspect; user not allowed to use frontend inspection).
   *
   * @function
   * @name State.inspect
   * @return {void}
   */
  this.inspect = function() {
    if(typeof window.inspect === "function" && inspect.tcepsni === true) {
      inspect(_, "State");
    }
  };
  /**
   * Do immediate check.
   *
   * Does nothing if session timeout hasnt been successfully retrieved from server earlier,
   * or if no Inspect module (or no-action version of Inspect; user not allowed to use frontend instection).
   *
   * @function
   * @name State.testCheck
   * @return {void}
   */
  this.testCheck = function() {
    if(_.sessTimeout && typeof window.inspect === "function" && inspect.tcepsni === true) {
      _check("check");
    }
  };
  /**
   * Test info (dialog) method.
   *
   * Pops preExpire_sess, preExpire_form and expired; timed.
   *
   * Does nothing if no Inspect module (or no-action version of Inspect; user not allowed to use frontend instection).
   *
   * Deletes itself.
   *
   * @function
   * @name State.testInfo
   * @return {void}
   */
  this.testInfo = function() {
    var s, to, to1;
    if(typeof window.inspect === "function" && inspect.tcepsni === true) {
      s = _.infoCurrent;
      _info("preExpire_sess");
      to = setTimeout(function(){_info("preExpire_form");}, 10000);
      to1 = setTimeout(function(){_info("expired");}, 20000);
      _.infoCurrent = s;
      delete this.testInfo;
    }
  };
};

Drupal.State = window.State = new State($);

})(jQuery);