/**
 * @file
 * inspect variable dumper and error tracer.
 */

/*jslint browser: true, continue: true, indent: 2, newcap: true, nomen: true, plusplus: true, regexp: true, white: true, ass: true*/
/*global alert: false, confirm: false, console: false*/
/*global jQuery: false*/
/*global mixed: false, integer: false, float: false, array: false, collection: false*/

/**
 * inspect variable dumper and error tracer.
 *
 * Doesn't require jQuery, but these functions will fail silently without it:
 * .log(), .traceLog(), .events(), .eventsLog(), .eventsGet().
 *
 * @param {function} [$]
 *  - jQuery is optional
 */
(function($) {
  'use strict';
  /**
   * @ignore
   */
  window.inspect = (function() {
    var _opts = ['depth', 'protos', 'func_body', 'message', 'type', 'severity', 'output_to', 'wrappers'], _nOpts = _opts.length,
    _strQt = ['`', '`'],
    _loc,
    _dmnRgx = new RegExp((_loc = window.location.href).replace(/^(https?:\/\/[^\/]+)\/.*$/, '$1').replace(/([\/\.\-])/g, '\\' + '$1')),
    _sev = [ // syslog severity, RFC 5424.
      'emergency',
      'alert',
      'critical',
      'error',
      'warning',
      'notice',
      'info',
      'debug'
    ],
    _logNo = 0,
    _conf = {
      inspect: {
        func_body: false
      },
      localizeFunc: null,
      logMessage: {
        url: '/inspect/ajax',
        argsUrl: [
          'sessionCounter',
          'logNo',
          'severity'
        ]
        //argsGET: {
        //  property: 'GET arg name'
        //}
      }
    },
    _ua,
    /**
     * Object.keys() for IE<8.
     *
     * @ignore
     * @private
     * @param {object} o
     * @return {array}
     */
    _oKeys = Object.keys ? function(o) {
      return Object.keys(o);
    } : function(o) {
      var k, a = [];
      for (k in o) {
        if (o.hasOwnProperty(k)) {
          a.push(k);
        }
      }
      return a;
    },
    /**
     * Resolve options argument.
     *
     * Copies arg u, if object.
     *
     * @ignore
     * @private
     * @param {mixed} u
     * @return {object}
     */
    _rslv = function(u) {
      var o = {}, i, t;
      if (!u || (t = typeof u) !== 'object') {
        for (i = 0; i < _nOpts; i++) {
          o[ _opts[i] ] = undefined;
        }
        if (u) {
          switch (t) {
            case 'number':
              if (isFinite(u)) {
                o.depth = u;
              }
              break;
            case 'string':
              switch (u) {
                case 'protos':
                  o.protos = true;
                  break;
                case 'func_body':
                  o.func_body = true;
                  break;
                default:
                  o.message = u;
              }
              break;
            case 'boolean':
              if (u) {
                o.protos = true;
              }
              break;
          }
        }
        else if (u === 0) {
          o.depth = 0;
        }
      }
      else {
        for (i = 0; i < _nOpts; i++) {
          o[ _opts[i] ] = u.hasOwnProperty(_opts[i]) ? u[ _opts[i] ] : undefined;
        }
        // 'category' alias of 'type'.
        if (!o.type && u.category) {
          o.type = u.category;
        }
        if (o.hasOwnProperty('output_to')) {
          switch ('' + o.output_to) {
            case '1':
            case 'console':
              o.output_to = 1;
              break;
            case '2':
            case 'log':
              o.output_to = 2;
              break;
          }
        }
      }
      return o;
    },
    /**
     * Send message to browser console, if exists.
     *
     * @ignore
     * @private
     * @param {mixed} [ms]
     * @return {void}
     */
    _cnsl = function(ms) {
      try { // detecting console is not possible, thus try-catch
        console.log('' + ms);
      }
      catch (er) {
      }
    },
    /**
     * All native types are reported in lowercase (like native typeof does).
     * If given no arguments: returns 'inspect'.
     * Types are:
     * - native, typeof: object string number
     * - native, corrected: function array date regexp image
     * - window, document, document.documentElement (not lowercase)
     * - element, checked via .getAttributeNode
     * - text node: textNode
     * - attribute node: attributeNode
     * - event: event (native and prototyped W3C Event and native IE event)
     * - jquery
     * - emptyish and bad: undefined, null, NaN, infinite
     * - custom or prototyped native: all classes having a typeOf() method.
     * RegExp is an object of type regexp (not a function -
     * gecko/webkit/chromium).
     * Does not check if Date object is NaN.
     *
     * @ignore
     * @private
     * @param {mixed} u
     * @return {string}
     */
    _typeOf = function(u) {
      var t = typeof u;
      if (!arguments.length) {
        return 'inspect';
      }
      switch (t) {
        case 'boolean':
        case 'string':
          return t;
        case 'number':
          return isFinite(u) ? t : (isNaN(u) ? 'NaN' : 'infinite');
        case 'object':
          if (u === null) {
            return 'null';
          }
          // Accessing properties of object may err for various reasons, like
          // missing permission (Gecko).
          try {
            if (u.typeOf && typeof u.typeOf === 'function') {
              return u.typeOf();
            }
            if (typeof u.length === 'number' && !(u.propertyIsEnumerable('length')) && typeof u.splice === 'function'
              && Object.prototype.toString.call(u) === '[object Array]'
            ) {
              return 'array';
            }
            if (u === window) {
              return 'window';
            }
            if (u === document) {
              return 'document';
            }
            if (u === document.documentElement) {
              return 'document.documentElement';
            }
            if (u.getAttributeNode) { // element
              // document has getElementsByTagName, but not getAttributeNode
              // - document.documentElement has both.
              return u.tagName.toLowerCase === 'img' ? 'image' : 'element';
            }
            if (u.nodeType) {
              switch (u.nodeType) {
                case 3:
                  return 'textNode';
                case 2:
                  return 'attributeNode';
              }
              return 'otherNode';
            }
            if (typeof u.stopPropagation === 'function' ||
              (u.cancelBubble !== undefined && typeof u.cancelBubble !== 'function' &&
                typeof u.boundElements === 'object')) {
              return 'event';
            }
            if (typeof u.getUTCMilliseconds === 'function') {
              return 'date';
            }
            if (typeof u.exec === 'function' && typeof u.test === 'function') {
              return 'regexp';
            }
            if (u.hspace && typeof u.hspace !== 'function') {
              return 'image';
            }
            if (u.jquery && typeof u.jquery === 'string' && !u.hasOwnProperty('jquery')) {
              return 'jquery';
            }
          }
          catch (er) {
          }
          return t;
        case 'function':
          //  gecko and webkit reports RegExp as function instead of object
          return (u.constructor === RegExp || (typeof u.exec === 'function' && typeof u.test === 'function')) ?
            'regexp' : t;
      }
      return t;
    },
    /**
     * Analyze variable.
     *
     * @ignore
     * @private
     * @param {mixed} u
     * @param {boolean} [protos]
     *  - default: false
     *  - include non-native prototypals
     * @param {boolean} [func_body]
     *  - default: false
     *  - print bodies of functions
     * @param {integer} [max]
     *  - default: 10
     *  - depth limiter
     * @param {integer} [depth]
     *  - default: zero
     *  - current depth
     * @return {string}
     */
    _nspct = function(u, protos, func_body, max, depth) {
      var m = max !== undefined ? max : 10, d = depth || 0, fb = func_body,
        t, isArr, p, pT, v, buf = '', s, isProt, nInst = 0, nProt = 0, ind, fInd, i, em;
      if (m > 10 || m < 0) {
        m = 10;
      }
      if (d < 11) {
        if (u === undefined) {
          return '(undefined)\n';
        }
        // Override the falsy func_body default ------
        if (!d && fb === undefined && _conf.inspect.func_body) {
          fb = true;
        }
        // Check type by typeof ----------------------
        switch ((t = typeof u)) {
          case 'object':
            if (u === null) {
              return '(object) null\n';
            }
            break;
          case 'boolean':
            return '(' + t + ') ' + u.toString() + '\n';
          case 'string':
            return '(' + t + ':' + u.length + ') ' + _strQt[0] +
              u.replace(/\n/g, '_NL_').replace(/\r/g, '_CR_').replace(/\t/g, '_TB_') +
              _strQt[1] + '\n';
        }
        // Check by typeOf() -------------------------
        ind = new Array(d + 2).join('.  ');
        switch ((t = _typeOf(u))) {
          case 'number':
            return '(' + t + ') ' + u + '\n';
          case 'NaN':
            return '(NaN) NaN\n';
          case 'infinite':
            return '(' + t + ') infinite\n';
          case 'regexp':
            return '(' + t + ') ' + u.toString() + '\n';
          case 'date':
            return '(date) ' + (u.toISOString ? u.toISOString() : u) + '\n';
          case 'function':
            // Find static members of the function.
            try {
              for (p in u) {
                if (u.hasOwnProperty(p) && p !== 'prototype') {
                  ++nInst;
                  // Recursion.
                  buf += ind + p + ': ' + _nspct(u[p], protos, fb, m - 1, d + 1);
                }
              }
            }
            catch (er1) {
            }
            v = u.toString();
            // Remove indentation of function as a whole.
            if (fb && (i = (v = v.replace(/\r/g, '').replace(/\n$/, '')).search(/[\ ]+\}$/)) > -1) {
              v = v.replace(new RegExp('\\n' + new Array(v.length - i).join('\\ '), 'g'), '\n');
            }
            return '(' + t + ':' + nInst + ') {\n' +
              (fInd = new Array(d + 2).join('   ')) +
              (!fb ? v.substr(0, v.indexOf(')') + 1) :
              v.replace(/\n/g, '\n' + fInd)
              ) +
              '\n' +
              buf + ind.replace(/\.\ \ $/, '') + '}\n';
          case 'window':
          case 'document':
          case 'document.documentElement':
            return '(' + t + ')\n';
          case 'element':
            // Events may have elements, with no tagName.
            return '(' + t + ':' + (u.tagName ? u.tagName.toLowerCase() : '') +
              ') ' + (u.id || '-') + '|' + (u.className || '-') +
              (!(u = u.getAttribute('name')) ? '' : ('|' + u)) + '\n';
          case 'textNode':
          case 'attributeNode':
          case 'otherNode':
            s = '(' + t + (t === 'otherNode' ? ':' + u.nodeType + ') ' : ' ');
            // Dumping nodeValue in IE<8 will fail.
            try {
              // Recursion.
              return s + _nspct(u.nodeValue, protos, fb, m - 1, d + 1);
            }
            catch (er2) {
            }
            return s + 'Unknown node value (IE)\n';
          case 'image':
            return '(image) ' + u.src + '\n';
          case 'event':
            break;
        }
        //  object -------------------------------------------------------
        isArr = (t === 'array');
        // Instance properties, prototypal attributes, prototypal methods.
        buf = ['', '', ''];
        em = '';
        try {
          pT = 'not iterable';
          for (p in u) {
            pT = _typeOf(v = u[p]);
            // Event misses hasOwnProperty method in some browsers, so we have
            // to check if hasOwnProperty exists.
            if (u.hasOwnProperty && u.hasOwnProperty(p)) {
              // Do always check for non-numeric instance properties if array
              // (error).
              if (isArr) {
                if (('' + p).search(/\D/) > -1) {
                  buf[0] += 'ERROR, non-numeric instance property [' + p + '] in array:\n';
                }
                else {
                  continue; // see next: if(isArr) {
                }
              }
              ++nInst;
              isProt = false;
            }
            else {
              ++nProt;
              if (!protos) {
                continue;
              }
              isProt = true;
            }
            if (m > 0) {
              s = p + ': ' +
                // Reference check.
                (v && pT === t && v === u ? (!d ? 'ref THIS\n' : 'ref SELF\n') :
                  _nspct(v, protos, fb, m - 1, d + 1)); // recursion
              if (!isProt) {
                buf[0] += (ind + s);
              }
              else {
                buf[ pT !== 'function' ? 1 : 2 ] += (ind + '... ' + s);
              }
            }
          }
          if (isArr) {
            nInst = u.length;
            if (m > 0) {
              for (p = 0; p < nInst; p++) {
                buf[0] += ind + p + ': ' +
                  // Reference check.
                  ((v = u[p]) && pT === t && v === u ? (!d ? 'ref THIS\n' : 'ref SELF\n') :
                    // Recursion.
                    _nspct(v, protos, fb, m - 1, d + 1));
              }
            }
          }
        }
        catch (er) {
          em = ' failed to inspect property ' + p + ', type ' + pT;
        }
        return '(' + t + ':' + nInst + (isArr ? '' : ('|' + nProt)) + ')' + em + (isArr ? ' [' : ' {') + '\n' +
          (m < 1 ? '' : (buf.join(''))) +
          new Array(d + 1).join('.  ') + (isArr ? ']' : '}') + '\n';
      }
      return 'RECURSION LIMIT\n';
    },
    /**
     * Find file and line.
     *
     * @ignore
     * @private
     * @param {integer} [wrappers]
     * @return {string}
     */
    _fL = function(wrappers) {
      var ar, le, i, v, p, wrps = wrappers || 0;
      //  Find call (first trace line outside this file).
      try {
        throw new Error();
      }
      catch (er) {
        ar = _trc(er);
      }
      if (typeof ar === 'string' && ar.indexOf('\n') > -1 && (le = (ar = ar.split('\n')).length)) {
        for (i = 0; i < le; i++) {
          if (i && (p = (v = ar[i]).lastIndexOf('@')) > -1 && v.indexOf('/inspect.js') === -1) {
            if (wrps) {
              if ((i += wrps) >= le) {
                return '@too_many_wrappers';
              }
              if ((p = (v = ar[i]).lastIndexOf('@')) > -1) {
                return '@wrappers_no_at';
              }
            }
            return v.substr(p);
          }
        }
      }
      return '@n/a';
    },
    /**
     * Log to backend.
     *
     *  Arg message as options object:
     *  - (string) caption
     *  - (string) message
     *  - (string) fileLine
     *  - (string) type: logging type
     *  - (string) category: alias of type
     *  - (string|integer) severity: syslog RFC 5424 number or equivalent string
     *    (error is 3 'error')
     *  - (string) kind: inspect|trace|info, default: info
     *  - (integer) wrappers
     *
     * Requires jQuery.ajax(); aborts quietly otherwise.
     *
     * @ignore
     * @private
     * @param {string|object} message
     *  - default: info
     * @param {string} [type]
     *  - logging type
     *  - default: info
     * @param {string|integer} [severity]
     *  - default: info
     *  - syslog RFC 5424 number or equivalent string (error is 3 'error')
     * @return void
     */
    _log = function(message, type, severity) {
      var u, k, le, i, ms = message, o = _conf.logMessage, url = o.url, cMax = 255, mMax = 102400, data = {
        logNo: (++_logNo),
        sessionCounter: 'i',
        type: ('' + type) || 'info',
        severity: severity, // 6 ~ info.
        kind: 'info',
        url: _loc,
        browser: _ua,
        caption: '',
        message: ms
      };
      // Requires jQuery.ajax().
      if (typeof $.ajax === 'function') {

        // Get page load id from cookie, if exists.
        if (typeof $.cookie === 'function' && (u = $.cookie('inspect__sc'))) {
          data.sessionCounter = u;
        }

        // Resolve arg message as options object.
        if (_typeOf(ms) === 'object') {
          for (k in ms) {
            if (ms.hasOwnProperty(k)) {
              switch (k) {
                case 'message':
                case 'type':
                case 'caption':
                case 'fileLine':
                  data[k] = '' + ms[k];
                  break;
                case 'category':
                  // Alias of 'type'.
                  if (ms[k]) {
                    data.type = '' + ms[k];
                  }
                  break;
                case 'severity':
                case 'wrappers':
                  data[k] = ms[k];
                  break;
                case 'kind':
                  switch ('' + ms[k]) {
                    case 'inspect':
                    case 'dump':
                    case 'trace':
                      data.kind = ms[k];
                      break;
                    default:
                      data.kind = 'info';
                  }
                  break;
                //default: Ignore.
              }
            }
          }
        }

        if (!data.fileLine) {
          data.fileLine = _fL(data.wrappers || 0);
        }
        delete data.wrappers;

        // Convert integer severity to string.
        if (_typeOf(u = data.severity) === 'number') {
          if (u > -1 && u < _sev.length) {
            u = _sev[u];
          }
          else {
            u = 'info';
          }
        }
        else if (!u) {
          u = 'info';
        }
        data.severity = u;

          // Truncate message and caption.
        data.message = data.message.substr(0, mMax);
        if (data.caption !== '') {
          data.caption = data.caption.substr(0, cMax);
        }
        else {
          delete data.caption;
        }


        // Url arguments.
        if ((u = o.argsUrl)) {
          le = u.length;
          for (i = 0; i < le; i++) {
            url += '/' + data[u[i]];
          }
        }
        // GET arguments.
        if ((u = o.argsGET)) {
          i = -1;
          for (k in u) {
            if (u.hasOwnProperty(k)) {
              url += ((!(++i) && o.url.indexOf('?') === -1) ? '?' : '&') + u[k] + '=' + data[k];
            }
          }
        }

        // Send request.
        $.ajax({
          url: url,
          type: 'POST',
          data: data,
          dataType: 'json', // expects json formatted response data
          cache: false,
          /**
           * @return {void}
           * @param {object} oResp
           *  - (boolean) success
           *  - (string) error
           * @param {string} textStatus
           * @param {object} jqXHR
           */
          success: function(oResp, textStatus, jqXHR) {
            /* // Uncomment for debugging.
             if(textStatus === 'success' &&  _typeOf(oResp) === 'object' && oResp.success && oResp.logNo) {
             //_cnsl('Log ' + oResp.logNo + ' succeeded');
             }
             else {
             inspect({
             textStatus: textStatus,
             response_data: oResp
             }, 'inspect.logMessage() failed');
             }
             */
          },
          error: function(jqXHR, textStatus, errorThrown) {
            /* // Uncomment for debugging.
             inspect({
             textStatus: textStatus,
             errorThrown: errorThrown
             }, 'inspect.logMessage() failed');
             */
          }
        });
      }
    },
    /**
     * Trace and stringify error.
     *
     * @ignore
     * @private
     * @param {Error} er
     * @return {string}
     */
    _trc = function(er) {
      var u, le, i, es = '' + er, s = es;
      if ((u = er.stack)) { // gecko, chromium
        if ((le = (u = (u.replace(/\r/, '').split(/\n/))).length)) {
          i = u[0] === es ? 1 : 0; // chromium first line is error toString()
          for (i; i < le; i++) {
            s += '\n  ' + u[i].replace(/^[\ ]+at\ /, '@').// chromium as gecko
              replace(_dmnRgx, '');
          }
        }
      }
      return s;
    },

    //  Public.

    /**
     * inspect() variable and send output to console log.
     *
     * inspect is a function, but documented as class due to jsdoc limitation.
     *
     * (object) options (any number of):
     *  - (integer) depth (default and absolute max: 10)
     *  - (string) message (default empty)
     *  - (boolean) protos (default not: do only report number of prototypal
     *    properties of objects)
     *  - (boolean) func_body (default not: do not print function body)
     *  - (boolean|integer) wrappers (default zero: this function is not wrapped
     *    in one or more local logging functions/methods)
     *
     * (integer) option:
     *  - interpreted as depth
     *
     * (boolean) option:
     *  - interpreted as protos
     *
     * (string) options:
     *  - 'protos' ~ report prototypal properties of objects
     *  - 'func_body' ~ print function body
     *  - otherwise interpreted as message
     *
     * Reference checks:
     *  - only child versus immediate parent
     *  - doesnt allow to recurse deeper than 10
     *
     * Prototype properties are marked with prefix '... ', function's static
     * members with prefix '.. '.
     * Detects if an Array property doesnt have numeric key.
     * Risky procedures are performed within try-catch.
     *
     * inspect doesn't require jQuery, but these functions will fail silently
     * without it: .log(), .traceLog(), .events(), .eventsLog(), .eventsGet().
     *
     * @name inspect
     * @constructor
     * @class
     * @singleton
     * @param {mixed} u
     * @param {object|integer|boolean|string} [options]
     *  - object: options
     *  - integer: depth
     *  - boolean: protos
     *  - string: message
     * @return {void}
     *  - logs to browser console, if exists
     */
    f = function(u, options) {
      var o = _rslv(options), ms = o.message || '';
      _cnsl(
        (!ms ? '' : (ms + ':\n')) + '[Inspect ' + _fL(o.wrappers) + ']\n' + _nspct(u, o.protos, o.func_body, o.depth)
        );
    };
    /**
     * Use for checking if that window.inspect is actually the one we are
     * looking for (see example).
     * The name of the property is just something unlikely to exist (and the
     * class name backwards).
     * @example
//  Check if inspect exists, and that it's the active sort.
if(typeof window.inspect === 'function' && inspect.tcepsni) {
  inspect('whatever');
}
     * @name inspect.tcepsni
     * @type {boolean}
     */
    f.tcepsni = true;
    /**
     * Use lowecase inspect.tcepsni instead.
     * @deprecated
     * @name inspect.tcepsnI
     * @type {boolean}
     */
    f.tcepsnI = true;

    /**
     * Override customizable settings (once).
     * @param {object} o
     * @return {void}
     */
    f.configure = function(o) {
      var k;
      for (k in o) {
        if (o.hasOwnProperty(k)) {
          _conf[k] = o[k];
        }
      }
      f.configure = function(){};
    };
    /**
     * Inspect variable and get output as string.
     *
     * @function
     * @name inspect.get
     * @param {mixed} u
     * @param {object|integer|boolean|string} [options]
     *  - see inspect()
     * @return {string}
     */
    f.get = function(u, options) {
      var o = _rslv(options), ms = o.message || '';
      return (!ms ? '' : (ms + ':\n')) + _nspct(u, o.protos, o.func_body, o.depth);
    };
    /**
     * Inspect variable and log output to backend.
     *
     * Additional options:
     *  - (string) type (default: inspect)
     *  - (string) severity (default: debug)
     *
     * Also logs current file and line, url, and browser plus useragent.
     *
     * @function
     * @name inspect.log
     * @param {mixed} u
     * @param {object|integer|boolean|string} [options]
     *  - see inspect()
     * @return {void}
     */
    f.log = function(u, options) {
      var o = _rslv(options);
      o.kind = 'dump';
      if (!o.type) {
        o.type = 'inspect';
      }
      if (!o.severity) {
        o.severity = 'debug';
      }
      o.fileLine = _fL(o.wrappers);
      if (o.message) {
        o.caption = o.message;
      }
      o.message = _nspct(u, o.protos, o.func_body, o.depth);
      _log(o);
    };
    /**
     * All native types are reported in lowercase (like native typeof does).
     * If given no arguments: returns 'inspect'.
     * Types are:
     * - native, typeof: object string number
     * - native, corrected: function array date regexp image
     * - window, document, document.documentElement (not lowercase)
     * - element, checked via .getAttributeNode
     * - text node: textNode
     * - attribute node: attributeNode
     * - event: event (native and prototyped W3C Event and native IE event)
     * - jquery
     * - emptyish and bad: undefined, null, NaN, infinite
     * - custom or prototyped native: all classes having a typeOf() method.
     * RegExp is an object of type regexp (not a function -
     * gecko/webkit/chromium).
     * Does not check if Date object is NaN.
     *
     * @function
     * @name inspect.typeOf
     * @param {mixed} u
     * @return {string}
     */
    f.typeOf = _typeOf;
    /**
     * Trace error and send output to console log.
     *
     * (object) options (any number of):
     *  - (string) message (default empty)
     *  - (boolean|integer) wrappers (default zero: this function is not wrapped
     *    in one or more local logging functions/methods)
     *
     * (string) options is interpreted as message.
     *
     * @example
try {
  throw new Error('¿Que pasa?');
}
catch(er) {
  inspect.trace(er, 'Class.method()');
}
     * @function
     * @name inspect.trace
     * @param {Error} er
     * @param {object|string} [options]
     *  - object: options
     *  - string: message
     * @return {void}
     *  - logs to browser console, if exists
     */
    f.trace = function(er, options) {
      var o = options || {}, s = o, wrps;
      if (typeof o === 'object') {
        s = o.message || '';
        wrps = o.wrappers;
      }
      _cnsl(
        (!s ? '' : (s + ':\n')) + '[Inspect trace ' + _fL(wrps) + ']\n' + _trc(er)
        );
    };
    /**
     * Trace error and get output as string.
     *
     * @function
     * @name inspect.traceGet
     * @param {Error} er
     * @param {object|string} [options]
     *  - object: options
     *  - string: message
     * @return {string}
     */
    f.traceGet = function(er, options) {
      var s = options || '';
      if (s && typeof s === 'object') {
        s = s.message || '';
      }
      return (!s ? '' : (s + ':\n')) + _trc(er);
    };
    /**
     * Trace error and log trace to backend.
     *
     * (object) options (any number of):
     *  - (string) message (default empty)
     *  - (integer) wrappers (default zero: inspect.traceLog() is not wrapped
     *    in one or more local logging functions/methods)
     *  - (string) type (default: inspect trace)
     *  - (string) severity (default: debug)
     *
     * (string) options is interpreted as message.
     *
     * Also logs current file and line, url, and browser plus useragent.
     *
     * @function
     * @name inspect.traceLog
     * @param {Error} er
     * @param {object|string} [options]
     *  - object: options
     *  - string: message
     * @return {void}
     */
    f.traceLog = function(er, options) {
      var o = _rslv(options);
      o.kind = 'trace';
      if (!o.type) {
        o.type = 'inspect trace';
      }
      if (!o.severity) {
        o.severity = 'debug';
      }
      o.fileLine = _fL(o.wrappers);
      if (o.message) {
        o.caption = o.message;
      }
      o.message = _trc(er);
      _log(o);
    };
    /**
     * Analyze function arguments, or an options object.
     *
     * @function
     * @name inspect.argsGet
     * @param {object|collection|array} args
     * @param {array} [names]
     *  - optional array of argument names
     * @return {string}
     */
    f.argsGet = function(args, names) {
      var a = args, t = _typeOf(a), le, keys, nKs, s, i;
      if (!a) {
        return 'falsy arg args';
      }
      if (t === 'array') {
        if (!(le = a.length)) {
          return '0: none';
        }
        // Use arg names as keys.
        nKs = !(keys = names) ? 0 : keys.length;
      }
      // arguments or object having no length.
      else if (!(le = ((keys = _oKeys(a)).length))) {
        return '0: none';
      }
      // arguments, arrayish
      else if (keys[0] === '0') {
        t = 'array';
        // Use arg names as keys.
        nKs = !(keys = names) ? 0 : keys.length;
      }
      else { // object
        nKs = le;
      }
      s = '' + le + ':';
      for (i = 0; i < le; i++) {
        s += (i ? ', ' : ' ') + '#' + i + (nKs <= i ? '' : ('(' + keys[i] + ')')) + ' ' +
          _nspct(t === 'array' ? a[i] : a[ keys[i] ], 0, 0, 0).
          replace(/\n/, ''); // get rid of trailing newline
      }
      return s;
    };
    /**
     * Inspect event listeners added via jQuery, and send output to console.
     *
     * List events of all elements matched by the jQuery selector, as array of
     * objects containing an element and a listener list.
     * Defaults to display bodies of functions (options func_body).
     *
     * @function
     * @name inspect.events
     * @param {string|object} selector
     *  - jQuery selector
     * @param {object|integer|boolean|string} [options]
     *  - see inspect()
     * @return {void}
     *  - logs to browser console, if exists
     */
    f.events = function(selector, options) {
      var o = _rslv(options), ms = o.message || '';
      o.message = '';
      _cnsl(
        (!ms ? '' : (ms + ':\n')) + '[Inspect events ' + _fL(o.wrappers) + ']\n' + f.eventsGet(selector, null, null, o)
        );
    };
    /**
     * Inspect event listeners added via jQuery, and get output as string.
     *
     * List events of all elements matched by the jQuery selector, as array of
     * objects containing an element and a listener list.
     * Defaults to display bodies of functions (options func_body).
     *
     * @function
     * @name inspect.eventsGet
     * @param {string|object} selector
     *  - jQuery selector
     * @param {object|integer|boolean|string} [options]
     *  - see inspect()
     * @return {string}
     */
    f.eventsGet = function(selector, options) {
      // Hidden 4th argument: resolved options.

      var o = arguments[3] || _rslv(options), ms = o.message || '', fb = o.func_body, s, u = selector, jq = $(u), d, a = [];
      if (!jq.get(0)) {
        s = 'arg selector, type ' + _typeOf(u) + ', value [' + u + '], matches no element(s)';
      }
      else {
        if (fb === undefined) {
          fb = true;
        }
        // No support for jQuery events listed under 'jquery[long hex number]'
        // bucket (observed via judy); cant replicate that pattern.
        // Using jQuery.each() to avoid reset values during iteration.
        jq.each(function() {
          a.push({
            element: this,
            listeners: (d = $(this).data()) && d.events && d.hasOwnProperty('events') ? d.events : null
          });
        });
        s = 'selector: ' + u + '\n' + _nspct(a, o.protos, fb, o.depth);
      }
      return (!ms ? '' : (ms + ':\n')) + s;
    };
    /**
     * Inspect event listeners added via jQuery, and log output to backend.
     *
     * List events of all elements matched by the jQuery selector, as array of
     * objects containing an element and a listener list.
     * Defaults to display bodies of functions (options func_body).
     *
     * Additional options:
     *  - (string) type (default: inspect)
     *  - (string) severity (default: debug)
     *
     * (string) options is interpreted as message.
     *
     * Also logs current file and line, url, and browser plus useragent.
     *
     * @function
     * @name inspect.eventsLog
     * @param {string|object} selector
     *  - jQuery selector
     * @param {object|integer|boolean|string} [options]
     *  - see inspect()
     * @return {void}
     */
    f.eventsLog = function(selector, options) {
      var o = _rslv(options);
      o.kind = 'inspect';
      if (!o.type) {
        o.type = 'inspect events';
      }
      if (!o.severity) {
        o.severity = 'debug';
      }
      o.fileLine = _fL(o.wrappers);
      if (o.message) {
        o.caption = o.message;
      }
      o.message = f.eventsGet(selector, null, null, o);
      _log(o);
    };
    /**
     * Send message to browser console, if exists.
     *
     * @function
     * @name inspect.console
     * @param {mixed} [ms]
     * @return {void}
     */
    f.console = _cnsl;
    /**
     * Log message to backend.
     *
     *  Arg message as options object:
     *  - (string) caption
     *  - (string) message
     *  - (string) fileLine
     *  - (string) type: logging type
     *  - (string) category: alias of type
     *  - (string|integer) severity: syslog RFC 5424 number or equivalent string
     *    (error is 3 'error')
     *  - (string) kind: inspect|trace|info, default: 'info'
     *  - (integer) wrappers
     *
     * @function
     * @name inspect.logMessage
     * @param {string|object} message
     *  - object: options list
     * @param {string} [type]
     *  - logging type
     *  - default: info
     * @param {string|integer} [severity]
     *  - default: info
     *  - syslog RFC 5424 number or equivalent string (error is 3 'error')
     * @return void
     */
    f.logMessage = _log;
    /**
     * Log error trace or variable inspection to console and/or backend log.
     *
     * Options like inspect(), except:
     *  - (integer) output_to: bitmask; 1 ~ console | 2 ~ backend log | 3 ~ both
     *    (default)
     *  - (string) output_to: 'console' | 'log'; default both
     *
     * @function
     * @name inspect.errorHandler
     * @param {Error} [error]
     * @param {mixed} [variable]
     * @param {object|integer|boolean|string} [options]
     * @return {void}
     */
    f.errorHandler = function(error, variable, options) {
      var o = _rslv(options), ms = o.message || '', to = o.output_to || 3,
        // Shan't add one, because _fL() looks for 'inspect', and that also
        // matches this function.
        wrps = o.wrappers;
      if (to !== 2) {
        if (error) {
          _cnsl(
            (!ms ? '' : (ms + ':\n')) + '[Inspect trace ' + _fL(wrps) + ']\n' + _trc(error)
          );
        }
        else {
          _cnsl(
            (!ms ? '' : (ms + ':\n')) + '[Inspect ' + _fL(wrps) + ']\n' + _nspct(variable, o.protos, o.func_body, o.depth)
          );
        }
      }
      if (to > 1) {
        if (error) {
          if (!o.type) {
            o.type = 'inspect trace';
          }
          o.messsage = _trc(error);
        }
        else {
          if (!o.type) {
            o.type = 'inspect';
          }
          o.messsage = _nspct(variable, o.protos, o.func_body, o.depth);
        }
        if (!o.severity) {
          o.severity = 'error';
        }
        o.fileLine = _fL(o.wrappers);
        o.caption = ms;
        _log(o);
      }
    };
    /**
     * Internal string localizer.
     *
     * @function
     * @name inspect.local
     * @param {mixed} str
     * @param {object} [replacers]
     * @returns {string}
     */
    f.local = function(str, replacers) {
      var s = '' + str, u = _conf.localizeFunc, k;
      if (u) {
        return u(s, replacers);
      }
      if ((u = replacers)) {
        for (k in u) {
          if (u.hasOwnProperty(k)) {
            s = s.replace(new RegExp(k, 'g'), k[u]);
          }
        }
      }
      return s;
    };

    // inspect.browser.
    (function() {
      var u = window.navigator, ua = 'no window.navigator.userAgent', x, y, nm, v = '?', b = {}, s;
      if (u && typeof u === 'object' && (u = u.userAgent) && typeof u === 'string') {
        if ((s = (ua = u).toLowerCase()).indexOf(x = 'msie') > 0) {
          nm = x;
          b.trident = true;
        }
        else if (s.indexOf('trident') > 0) {// IE>=11.
          nm = x; // Also 'msie'.
          b[nm] = (x = /^.+; rv:(\d+\.\d+)[;\)].+$/).test(s) ? parseFloat(v = s.replace(x, '$1')) : true;
          b.trident = true;
        }
        else if (s.indexOf(x = 'chrome') > 0 || s.indexOf(x = 'safari') > 0) {
          nm = x;
          b.webkit = (y = /^.+webkit\/(\d+\.\d+)[^\d].+$/).test(s) ? parseFloat(s.replace(y, '$1')) : true;
        }
        else if (s.indexOf(x = 'firefox') > 0) {
          nm = x;
          b.mozilla = b.gecko = true;
        }
        else if (s.indexOf(x = 'opera') > 0) {
          nm = x;
        }
        if (nm) {
          if (v === '?') {// Trident (IE>=11) already done.
            b[nm] = (x = new RegExp('^.+' + nm + '.(\\d+\\.\\d+)([^\\d].+)?$')).test(s) ? parseFloat(v = s.replace(x, '$1')) : true;
          }
        }
        else {
          nm = 'unknown';
        }
        b.type = nm;
        b.version = v;
        b.userAgent = ua;
      }
      _ua = nm + ' ' + v + ' [' + ua + ']';
      /**
       * Lists browser version (as float) by useragent identifier.
       *
       * Supported browsers/identifiers:
       *  - msie ~ Internet Explorer
       *  - firefox
       *  - mozilla ~ Firefox, but version is simply true (not float)
       *  - chrome ~ Google Chrome
       *  - safari
       *  - webkit ~ Google Chrome or Apple Safari, version is (Apple)Webkit
       *    version as float
       *  - opera
       *  - type ~ identifier as string
       *  - version ~ version number as string (for Firefox: the Firefox
       *    version, not the Gecko version).
       *  - userAgent ~ raw useragent
       *
       * @example
//  Check if browser is Internet Explorer < 8
var ie;
if((ie = inspect.browser.msie) && ie < 8) {
  inspect.console('Get a decent browser, bro´');
}
       * @name inspect.browser
       * @type {object}
       */
      f.browser = b;
    })();

    return f;
  })();
})(window.jQuery);
