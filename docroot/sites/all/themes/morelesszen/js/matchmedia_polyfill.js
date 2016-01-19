/*
 * (c) 2014 more onion
 *
 * this polyfill enables you to use window.matchMedia(q).matches
 * with IE 7 and IE 8 -- it resembles the media query behaviour
 * and enables you to test for min-width and max-width
 *
 * be sure not to load any other polyfills before this one :)
 *
 * depends on Respond.js for some querying and regexp
 * depends on Modernizr for media query test
 * make sure to load Respond.js and Modernizr before this file
 *
 * credits: quite a lot of code taken from Respond.js
 */
(function(w) {
  "use strict";
  // @TODO better test for Respond
  // only try to polyfill if we have Respond.js and no media query support
  // in this case the respondjs polyfill for matchMedia falls too short
  //
  // therefor we override any existing window.matchMedia as it could only be
  // a polyfill
  if (typeof w.respond !== "undefined" &&
      typeof Modernizr !== "undefined" && (!Modernizr.mq('only all') || typeof window.matchMedia === "undefined")) {
    window.matchMedia = function ( q ) {
      var eachq,
        eql,
        thisq,
        doc = w.document,
        docElem = doc.documentElement,
        mediastyles = [],
        respond = w.respond;

      eachq = q.split( "," );
      eql = eachq.length;

      for( var j = 0; j < eql; j++ ){
        thisq = eachq[ j ];

        if(respond.mediaQueriesSupported) {
          return;
        }

        mediastyles.push( {
          media : thisq.split( "(" )[ 0 ].match( respond.regex.only ) && RegExp.$2 || "all",
          hasquery : thisq.indexOf("(") > -1,
          minw : thisq.match( respond.regex.minw ) && parseFloat( RegExp.$1 ) + ( RegExp.$2 || "" ),
          maxw : thisq.match( respond.regex.maxw ) && parseFloat( RegExp.$1 ) + ( RegExp.$2 || "" )
        } );
      }

      function queryMatches() {
        for( var i in mediastyles ) {
          if( mediastyles.hasOwnProperty( i ) ){
            var thisstyle = mediastyles[ i ],
              min = thisstyle.minw,
              max = thisstyle.maxw,
              minnull = min === null,
              maxnull = max === null,
              em = "em",
              name = "clientWidth",
              docElemProp = docElem[ name ],
              currWidth = doc.compatMode === "CSS1Compat" && docElemProp || doc.body[ name ] || docElemProp;

            // TODO parse em
            if( !!min ){
              min = parseFloat( min ) * ( min.indexOf( em ) > -1 ? ( eminpx || respond.getEmValue() ) : 1 );
            }
            if( !!max ){
              max = parseFloat( max ) * ( max.indexOf( em ) > -1 ? ( eminpx || respond.getEmValue() ) : 1 );
            }

            // if there a media query, or min or max is not null,
            // and if either is present, they're true
            if( thisstyle.hasquery && ( !minnull || !maxnull ) && ( minnull || currWidth >= min ) && ( maxnull || currWidth <= max ) ){
              return true;
            }
          }
        }
        return false;
      }

      return {
        matches: queryMatches(),
        media: q
      };
    }
  }
})(this);
