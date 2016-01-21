/**
 * cqBeautifyXML - javascript plugin
 *
 * based on
 *
 * vkBeautifyXML
 * Version - 1.0.beta
 * Copyright (c) 2012 Vadim Kiryukhin
 * vkiryukhin @ gmail.com
 * http://www.eslinstructor.net/cqBeautifyxml/
 *
 * Dual licensed under the MIT and GPL licenses:
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html
 *
 *	.cqBeautifyxml(xml [,collapseWS ])
 *
 * PARAMETERS:
 *
 *	@xml  			- xml to beautify;
 * 	@collapseWS		- bool (optional);
 *					  flag, which instruct application to remove white spaces
 *					  between open and close tags if no other characters are found;
 *
 * USAGE:
 *
 *	cqBeautifyxml(xml);
 *	cqBeautifyxml(xml,true);
 *
 */

(function () {

  window.cqBeautifyxml = function (xml, collapseWS) {
    collapseWS = true;

    xml = xml.replace(/(\r\n|\n|\r)/gm, "").replace(/\s\s+/g, " ")
    var shift = ['\n'], // array of shifts
            deep = 0,
            str = '',
            step = '    ', //4 spaces
            inComment = false,
            maxdeep = 50, // nesting level
            ix = 0,
            /* preserves white spaces, doubles "/n"  */
            //ar = xml.replace(/</g,"~#~<").split('~#~'),
            /* eats white spaces between > <, so that <a>   </a> becomes <a></a> */
            //ar = xml.replace(/>\s{0,}</g,"><").replace(/</g,"~#~<").split('~#~'),

            ar = collapseWS ? ar = xml.replace(/>\s{0,}</g, "><").replace(/</g, "~#~<").split('~#~')
            : xml.replace(/</g, "~#~<").split('~#~'),
            len = ar.length;

    var inNonFormatted = function (str) {
      var regExps, i;

      regExps = [/<!/, /<p/, /<hint/];
      for (i = 0; i < regExps.length; i++) {
        if (str.search(regExps[i]) > -1) {
          return 'start';
        }
      }

      regExps = [/-->/, /\]>/, /!DOCTYPE/, /<\/p/, /<\/hint/];
      for (i = 0; i < regExps.length; i++) {
        if (str.search(regExps[i]) > -1) {
          return 'end';
        }
      }

      return false;
    }
    /* initialize array with shifts */
    for (ix = 0; ix < maxdeep; ix++) {
      shift.push(shift[ix] + step);
    }

    for (ix = 0; ix < len; ix++) {
      /* start comment or <![CDATA[...]]> or <!DOCTYPE*/
      if (inNonFormatted(ar[ix]) === 'start') {
        str += shift[deep] + ar[ix];
        inComment = true;
        /* end comment  or <![CDATA[...]]> */
        if (inNonFormatted(ar[ix]) === 'end') {
          inComment = false;
        }
      } else
      /* end comment  or <![CDATA[...]]> */
      if (inNonFormatted(ar[ix]) === 'end') {
        str += ar[ix];
        inComment = false;
      } else
      /* <elm></elm> */
      if (/^<\w/.exec(ar[ix - 1]) && /^<\/\w/.exec(ar[ix]) &&
              /^<\w+/.exec(ar[ix - 1]) === /^<\/\w+/.exec(ar[ix])[0].replace('/', '')) {
        str += ar[ix];
        if (!inComment)
          deep--;
      } else
      /* <elm> */
      if (ar[ix].search(/<\w/) > -1 && ar[ix].search(/<\//) === -1 && ar[ix].search(/\/>/) === -1) {
        str = !inComment ? str += shift[deep++] + ar[ix] : str += ar[ix];
      } else
      /* <elm>...</elm> */
      if (ar[ix].search(/<\w/) > -1 && ar[ix].search(/<\//) > -1) {
        str = !inComment ? str += shift[deep] + ar[ix] : str += ar[ix];
      } else
      /* </elm> */
      if (ar[ix].search(/<\//) > -1) {
        str = !inComment ? str += shift[--deep] + ar[ix] : str += ar[ix];
      } else
      /* <elm/> */
      if (ar[ix].search(/\/>/) > -1) {
        str = !inComment ? str += shift[deep] + ar[ix] : str += ar[ix];
      } else
      /* <? xml ... ?> */
      if (ar[ix].search(/<\?/) > -1) {
        str += shift[deep] + ar[ix];
      } else {
        str += ar[ix];
      }
    }
    return  (str[0] === '\n') ? str.slice(1) : str;
  }

})();

