/**
 * @file
 * Inspect output formatter.
 */

'use strict';
/*jslint browser: true, continue: true, indent: 2, newcap: true, nomen: true, plusplus: true, regexp: true, white: true; ass: true*/
/*global alert: false, confirm: false, console: false*/
/*global jQuery: false*/
/*global mixed: false, integer: false, float: false, array: false, collection: false*/

/**
 * Formats inspection outputs.
 *
 * Doesn't err without jQuery, just aborts.
 *
 * @param {function} [$]
 *  - jQuery is optional
 */
(function($) {
  if ($) {
    $(document).bind('ready', function() {
      // jQuery().bind('ready') is another queue than jQuery().ready().
      var _cls = 'module-inspect-collapsible', _shw = 1, _fld = 1;
      if(!window.inspect ||
        // IE<9 cannot handle folding.
        (inspect.browser.msie && inspect.browser.msie < 9)) {
        return;
      }
      /**
       * Toggle key path display.
       *
       * @param {element} that
       * @return {void}
       */
      inspect.tglKyPth = function(that) {
        var jqFrm = $('form', that), elm, w;
        if(jqFrm.css('display') === 'none') {
          elm = ($('input', that).
            css('width', (w = $('span', that).innerWidth()) + 'px')).get(0);
          $('form > div', that).css('width', (w + 20) + 'px');
          jqFrm.css('display', 'inline');
          elm.focus(); // unselect double clicked word
        }
        else {
          jqFrm.css('display', 'none');
        }
      };
      /**
       * Stop propagation.
       *
       * @param {Event} [e]
       * @return {void}
       */
      inspect.stpPrpgtn = function(e) {
        if(e && e.stopPropagation) {
          e.stopPropagation();
        }
        else {
          window.event.cancelBubble = true;
        }
      };
      /**
       * Toggle display of all inspection outputs on current page.
       *
       * This function is always implemented, even for no-action no-fold.
       *
       * @function
       * @name inspect.toggleOutput
       * @return {void}
       */
      inspect.toggleDisplay = function() {
        var u;
        if (_shw > 0) {
          $('.' + _cls).hide();
        }
        else if (_fld < 0) {
          $('pre.' + _cls).show();
        }
        else {
          $('pre.' + _cls).each(function() {
            $((u = $(this).prev('div.' + _cls).get(0)) ? u : this).show();
          });
        }
        _shw *= -1;
      };
      /**
       * Toggle inspection folding, unless folding is disabled.
       *
       * Folding is not supported for IE<9.
       *
       * @function
       * @name inspect.toggleFolding
       * @return {void}
       */
      inspect.toggleFolding = function() {
        if (_fld > 0) {
          $('div.' + _cls).hide();
          $('pre.' + _cls).show();
        }
        else {
          $('pre.' + _cls).hide();
          $('div.' + _cls).show();
        }
        _fld *= -1;
        _shw = 1;
      };

      (function() {
        var cls = _cls, // 'module-inspect-collapsible'
          clsFrmtd = 'module-inspect-formatted',
          linesMax = {
            fold: 10000,
            frmt: 25000
          },
          doFld = 1,
          nl = '<br/>', ds = '<div>', de = '</div>', sp = '<span>', se = '</span>',
          ts = '<span title="session : page load : request : log">',
          ps = '<div class="inspect-cllpsbl cllpsd"><div class="inspect-prnt">',
          b1 = '<span class="inspect-xpnd">\u25b6' + se,
          b2 = '<span class="inspect-xpnd-all">\u25b6\u25b6' + se,
          cs = '<div class="inspect-chldrn">',
          cc = '<div class="inspect-chld">',
          kp = '<span class="inspect-kypth" onclick="inspect.tglKyPth(this);" title="',
          ssu = '<span title="utf8">',
          ssa = '<span title="ascii">',
          sst = '<span title="truncation" class="strng">',
          ssp = '<span title="path removed" class="strng">',
          k1 = '">' + sp, // span for measuring width
          k2 = se + '<form><div><input type="text" onclick="inspect.stpPrpgtn(event);" value="',
          k3 = '"/><div title="close">&#215;</div></div></form>',
          sw = '<span class="warn">',
          st = '<span title="truncation" class="trunc">',
          clsTgl = 'module-inspect-toggle',
          rprts = location.href.indexOf('admin/reports/event/') > -1,
          fTgl = window.inspect[ 'toggle' + (rprts ? 'Folding' : 'Display') ],
          tg = '<div class="' + clsTgl + '"><div title="toggle ' + (rprts ? 'folding' : 'display') + '">&#0161;!' + de + de,
          rgx =  {
            a: /^\.\s\s/, // indentation start
            b: /^(\.\s\s)+(.*?):\s\(.+$/, // find bucket name (and remove indentation from it)
            c: /\['?$/, // key path array started
            d: /:\d+(\|\d+)?\)\ \{$/, // start of object ends with ':N) {' or ':N|N) {' (javascript only; counts prototypals)
            e: /rray:\d+\)\ \[$/, // start of array ends with array:N) [
            f: /\(arra[y]:/,
            g: /^(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?(\.\s\s)?.*$/,
            h: /\['$|\-\>$/,
            i: /\['(\d+)'\]/g,
            j: /^(\.[ ]{2})+/,
            k: /\(string:(\d+)\|(\d+)\) `(.*)`$/, // String lengths.
            l: /\(string:(\d+)\|(\d+)\|(\d+)\) `(.*)\.{3}`$/, // String truncation.
            m: /\(string:(\d+)\|(\d+)\|(\-|\d+)\|\!\) `\.{3}(.*)`$/, // String truncation + path removal.
            n: /\(([a-zA-Z_][a-zA-Z\d_]*):(\d+)\) (\[\.{3}\]|\{\.{3}\})/, // Container truncation.
            o: /\(([a-zA-Z_][a-zA-Z\d_]*)(:)?([\d\?\|]+)*\) \*([A-Z\d_]+)\*/,
            p: /\['(\d+)'\]/g,
            q: /^Inspection aborted/,
            r: /_NL_/g,
            s: /([\[\ ])([a-zA-Z\d]+:\d+:\d+:\d+)([\ \]])/,
            t: /unction:\d+\)\ \{$/, // start of function ends with 'function:N) {' (javascript only)
            u: /\r\n/g
          },
          rpl = {
            g: '$1$2$3$4$5$6$7$8$9$10'
          },
          //clsTrc = 'module-inspect-trace',
          clsPrf = 'inspect-profile', // profiler
          /**
           * Folding of backend generated dump outputs on page.
           * @ignore
           * @param {array} lines
           * @param {integer} first
           * @param {integer} last
           * @param {array} path
           * @param {boolean} top
           * @param {boolean} frntnd
           * @return {array|boolean}
           *  - [ (string) buffer, (integer) first, (boolean|undefined) deep ]
           */
          fold = function(lines, first, last, path, top, frntnd) {
            var start = first, j = 0, buf = '', line, obj, arr, i, stop, ndnt, a, deep, pth = path.concat(), kyPth,
              oStrt = !frntnd ? '->' : '.', aStrt = !frntnd ? '[\'' : '[', aEnd = !frntnd ? '\']' : ']', k, fnc, p, fLine;
            while(start <= last && (++j) < 10000) { // not linesMax, simply loop limiter
              obj = arr = stop = false;
              line = lines[start];
              //  Key path generation: find bucket name
              pth.push(
                (!rgx.a.test(line) ? '' : // if doesnt start with indent; name is empty
                  line.replace(rgx.b, '$2')) + // else remove indentation; there is a name
                  (!rgx.c.test(pth[pth.length - 1]) ? '' : aEnd) // possibly end array key path
              );
              //  javascript function; must be checked before object, because object regex cannot be exact
              if(frntnd && rgx.t.test(line)) {
                if(last === start) {
                  return false; // container start and end cant be on same line
                }
                deep = true;
                //  add container type
                pth[ pth.length - 1 ] += '.';

                buf += ps +
                  (ndnt = line.indexOf('.  ') !== 0 ? '' : line.replace(rgx.g, rpl.g)) + // remove indentation
                  b1;
                ++start;
                //  find end
                for(i = start; i < last + 1; i++) {
                  if(lines[i] === ndnt + '}') {
                    stop = i;
                    //  find function self
                    fnc = '';
                    for(k = start; k < stop; k++) {
                      if(!rgx.a.test(lines[k])) {
                        fnc += (k > start ? nl : '') +
                          //  turn tripple indentation of function as a whole into double and non-breaking
                          (ndnt + '.  ').replace(/\./g, '&nbsp;') + (fLine = lines[k].substr(ndnt.length + 3)).
                          replace( // make function's internal indentation non-breaking
                            new RegExp('^' + new Array(p = fLine.search(/[^\ ]/) + 1).join('\\ ') + '([^\\ ].*)$'),
                            new Array(p).join('&nbsp;') + '$1'
                          );
                      }
                      else {
                        break;
                      }
                    }
                    //  function static members
                    if(!(a = fold(lines, k, stop - 1, pth, false, frntnd))) {
                      return false; // error
                    }
                    buf += (!a[2] ? '' : b2) + '&nbsp;' +
                      (!top ?
                        (kp + (kyPth = pth.join('').replace(rgx.h, '').replace(rgx.i, '[$1]')) +
                          k1 + kyPth + k2 + kyPth + k3) :
                        sp // no key path for top var
                        ) +
                      line.replace(rgx.j, '') + se +
                      de + cs +
                      fnc +
                      a[0] +
                      cc +
                      lines[i] +
                      de + de + de;
                    pth.pop();
                    start = a[1] + 1; // 1 to prevent double ending
                    break;
                  }
                }
                if(!stop) {
                  return false; // error, found no ending } or ]
                }
              }
              //  object or array
              else if((obj = rgx.d.test(line)) || (arr = rgx.e.test(line))) {
                if(last === start) {
                  return false; // container start and end cant be on same line
                }
                deep = true;
                //  add container type
                pth[ pth.length - 1 ] += ((obj && !rgx.f.test(line)) ? oStrt : aStrt);

                buf += ps +
                  (ndnt = line.indexOf('.  ') !== 0 ? '' : line.replace(rgx.g, rpl.g)) + // remove indentation
                  b1;
                ++start;
                //  find end
                for(i = start; i < last + 1; i++) {
                  if((obj && lines[i] === ndnt + '}') || (arr && lines[i] === ndnt + ']')) {
                    stop = i;
                    if(!(a = fold(lines, start, stop - 1, pth, false, frntnd))) {
                      return false; // error
                    }
                    buf += (!a[2] ? '' : b2) + '&nbsp;' +
                      (!top ?
                        (kp + (kyPth = pth.join('').replace(rgx.h, '').replace(rgx.i, '[$1]')) +
                          k1 + kyPth + k2 + kyPth + k3) :
                        sp // no key path for top var
                        ) +
                      line.replace(rgx.j, '') + se +
                      de + cs +
                      a[0] +
                      cc +
                      lines[i] +
                      de + de + de;
                    pth.pop();
                    start = a[1] + 1; // 1 to prevent double ending
                    break;
                  }
                }
                if(!stop) {
                  return false; // error, found no ending } or ]
                }
              }
              //  non-container var
              else {
                ++start;
                ndnt = line.indexOf('.  ') !== 0 ? '' : line.replace(rgx.g, rpl.g); // remove indentation
                buf += cc +
                  (ndnt = line.indexOf('.  ') !== 0 ? '' : line.replace(rgx.g, rpl.g)) + // remove indentation
                  (!top ?
                    (kp + (kyPth = pth.join('').replace(rgx.p, '[$1]')) + k1 + kyPth + k2 + kyPth + k3) :
                    (!rgx.q.test(line) ? sp : sw) // no key path for top var
                    ) +
                  line.replace(/^(\.  )+/, '').
                    replace( // string lengths
                      rgx.k,
                      '(string:' + ssu + '$1' + se + '|' + ssa + '$2' + se + ') ' + st + '`' + se + '$3' + st + '`' + st
                    ).
                    replace( // highlight string truncation
                      rgx.l,
                      '(string:' + ssu + '$1' + se + '|' + ssa + '$2' + se + '|' + sst + '$3' + se + ') ' + st + '`' + se + '$4' + st + '...`' + st
                    ).
                    replace( // highlight path removal
                      rgx.m,
                      '(string:' + ssu + '$1' + se + '|' + ssa + '$2' + se + '|' + sst + '$3' + se + '|' + ssp + '!' + se + ') ' + st + '`...' + se + '$4' + st + '`' + st
                    ).
                    replace(rgx.n, '($1:$2) ' + st + '$3' + se). // highlight container truncation
                    replace(rgx.o, '($1$2$3) ' + sw + '*$4*' + se). // highlight error
                    replace(rgx.r, '_NL_<br/>').
                    //  Put simple value in container which isnt clickable.
                    replace(/^([^:]+):\ \(([^\)]+)\)(.+)$/, '$1: ($2) <span onclick="inspect.stpPrpgtn(event);">$3</span>') +
                  se + de;
                pth.pop();
              }
            }
            return [buf, start, deep];
          },
          prof = function(lines) {
            var s = '', le = lines.length, i, line, j = 0, ttl;
            for(i = 0; i < le; i++) {
              if(/^.+\|/.test(line = lines[i])) {
                //  truncate log, set as title
                if((ttl = !/^([\s\d]{0,4}\d)/.test(line) || /.+\|\s$/.test(line) ? '' : line.replace(/^.+\|\s([^\|]+)$/, '$1'))) {
                  line = line.replace(/^(.+\|\s)[^\|]+$/, '$1') + ttl.substr(0, 32);
                  ttl = ' title="' + ttl + '"';
                }
                s += '<div class="inspect-' + (((++j) % 2) ? 'odd' : 'even') + '"' + ttl + '>'; // even/odd
              }
              else {
                j = 0;
                s += ds;
              }
              s += line.replace(/\s/g, '\u00A0') + de;
            }
            return s;
          };

        /**
         * Also works on profile output.
         *
         * @param {boolean|integer} [find]
         *  - boolean true: work on all non-formatted inspection and profile
         *    outputs (in context or window)
         *  - otherwise ignored (like if used in jQuery().each(), where the
         *    first (index) argument is a number)
         * @param {element|jquery} [context]
         * @returns {void}
         */
        inspect.formatOutput = function(find, context) {
          var jqTgl, jqFrmtd, buf0 = '', buf = '', jq, lines, le,
            i, j = -1, a, nm = '', prf = false, frntnd;

          // Find all.
          if (find && typeof find === 'boolean') {
            (function() {
              var a = $('pre.' + cls, context).not('.' + clsFrmtd).get(), le = a.length, i;
              for (i = 0; i < le; ++i) {
                // Call formatOutput as method of a <pre> element.
                inspect.formatOutput.apply(a[i], []);
              }
            })();
            return;
          }

          // I am a <pre>.
          jq = $(this);

          // Reset the folding brake.
          doFld = 1;

          lines = jq.html().replace(rgx.u, '\n').split(/\n/);
          le = lines.length;
          frntnd = $(this).hasClass('inspect-frontend');

          // Mark done.
          jq.addClass(clsFrmtd);

          //  filter message (everything before [Inspect)
          for(i = 0; i < le; i++) {
            if(/^\[(frontend )?Inspect/.test(lines[i])) {
              j = i;
              nm = (lines[i]).replace(/^\[(frontend )?Inspect[^\]]*\]\s?(.+)?/, '$2'); // Find variable name, if any.
              break;
            }
          }
          lines[0] = lines[0].replace(rgx.s, '$1' + ts + '$2' + se + '$3'); // title on session:page:request:log
          if(j === -1) {
            return; // error
          }
          //  add lines before [Inspect...
          for(i = 0; i < j; i++) {
            buf0 += (!i ? '' : nl) + lines[i];
          }
          //  remove lines before [Inspect...
          lines.splice(0, j);
          buf0 += ds + lines[0]; // [Inspect...
          //  inspect profile: dont fold
          if(jq.hasClass(clsPrf)) {
            prf = true;
            lines.splice(0, 1);
            buf = buf0 + de + prof(lines);
          }
          else {
            //  remove file_line
            if(lines.length > 1 && // content truncation (Read more) may shorten output to a single line
              lines[1].indexOf('@') === 0) {
              buf += ds + lines[1] + de;
              lines.splice(1, 1);
            }
            if((le = lines.length) <= linesMax.frmt) {
              if(le > linesMax.fold) {
                doFld = 0;
                ps = ps.replace(/\ cllpsd/, '');
                b1 = b2 = '';
              }
              if(!(a = fold(lines, 1, le - 1, [nm], true, frntnd))) {
                return; // error
              }
              if(!doFld) {
                buf0 += nl + '* ' +
                  inspect.local(
                    'Folding off, number of output lines !le exceed max !max, try less depth',
                    {'!le':le,'!max':linesMax.fold}
                  ) + ' *';
              }
              else {
                buf0 += nl + '- <span title="' +
                  inspect.local('Maximum: folding !max0, formatting !max1.', {'!max0':linesMax.fold,'!max1':linesMax.frmt}) + '">' +
                  inspect.local('output lines !le', {'!le':le}) + se + ' -';
              }
              buf = buf0 + de + buf + a[0];// + de;
            }
          }
          // Insert unformatted pre/formatted div toggler.
          jqTgl = $(tg).insertBefore(jq);

          if(le <= linesMax.frmt) {
            // Hide unformatted pre.
            jq.hide();
            // Insert formatted div.
            jqFrmtd = $('<div class="' + cls + (!prf ? '' : (' ' + clsPrf)) + '">' + buf + de).insertBefore(jq);

            //  Adds button events, and locks width.
            setTimeout(function(){
              //  set .toggleFolding() or .toggleDisplay() on
              jqTgl.click(fTgl);

              if(doFld) {
                //  Fixate width of expansible outputs' outer container.
                (function() {
                  // Not profile output.
                  if(!jqFrmtd.hasClass(clsPrf)) {
                    var w, p;
                    jqFrmtd.css({
                      width: w = (jqFrmtd.innerWidth() - 20) + 'px',
                      'max-width': w
                    });
                    // Set expand/collapse events on all buttons.
                    $('div.inspect-cllpsbl', jqFrmtd.get(0)).each(function() {
                      var par = this,
                        bt = $('span.inspect-xpnd', par).get(0), btAll = $('span.inspect-xpnd-all', par).get(0),
                        btChldrn = [], chld;
                      $('div.inspect-cllpsbl', par).each(function(){
                        if((chld = $('span.inspect-xpnd', this).get(0))) {
                          btChldrn.push(chld);
                        }
                      });
                      $(bt).click(function(event, on, off){
                        var jq = $(par), show = on ? true : (off ? false : jq.hasClass('cllpsd'));
                        $(this).text(show ? '\u25bc' : '\u25b6');
                        jq[show ? 'removeClass' : 'addClass']('cllpsd');
                        if(!show && btAll) { // if hide: collapse sibling button
                          $(btAll).text('\u25b6\u25b6');
                        }
                      }).mouseover(function(){
                        $(this).addClass('hover');
                      }).mouseout(function(){
                        $(this).removeClass('hover');
                      });
                      //  works via the single level buttons
                      if(btAll) {
                        $(btAll).click(function(){
                          var show = $(this).text() === '\u25b6\u25b6';
                          if(show) {
                            $(this).text('\u25bc\u25bc');
                          }
                          $(bt).trigger('click', [show, !show]); // click sibling
                          $(btChldrn).each(function(){ // click children
                            var btAll; // local
                            $(this).trigger('click', [show, !show]);
                            //  if has expand-all sibling, make it show that all is expanded/collapsed
                            if((btAll = $(this).next('span.inspect-xpnd-all').get(0))) {
                              $(btAll).text(show ? '\u25bc\u25bc' : '\u25b6\u25b6');
                            }
                          });
                        }).mouseover(function(){
                          $(this).addClass('hover');
                        }).mouseout(function(){
                          $(this).removeClass('hover');
                        });
                      }
                    });
                  }
                  // Profile output.
                  else if((p = jqFrmtd.css('position')) !== 'absolute' && p !== 'fixed') {
                    jqFrmtd.css('position', 'absolute');
                    w = (jqFrmtd.innerWidth() - 12) + 'px';
                    jqFrmtd.css({
                      position: p,
                      width: w,
                      'max-width': 'auto'
                    });
                  }
                })();
              }
            },100);
          }
          else {
            jq.prepend(
              '* ' +
                inspect.local(
                  'Formatting off, number of output lines !le exceed max !max, try less depth',
                  {'!le':le,'!max':linesMax.frmt}
                ) +  ' *\n'
            );
          }
        };
        // Format all current inspection outputs.
        inspect.formatOutput(true);

        /**
         * @deprecated
         */
        inspect.formatInspectOutput = function(find, context) {
          inspect.formatOutput(find, context);
        };
        /**
         * @deprecated
         */
        inspect.formatTraceOutput = function() {};

      })();
    });
  }
})(window.jQuery);
