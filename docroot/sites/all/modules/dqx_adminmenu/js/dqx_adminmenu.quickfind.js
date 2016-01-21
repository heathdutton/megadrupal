
(function(){

  var $ = jQuery;
  var d = Drupal.dqx_adminmenu;
  d.quickfind = {};


  d.quickfind.SearchIndex = function(searchindex, $divRoot) {

    var str = null;

    this.setSearchString = function(str_new) {
      if (str_new != str) {
        str = str_new;
        for (var l=1; l<=3; ++l) {
          $('.dqx_adminmenu-quickfind-m' + l, $divRoot).removeClass('dqx_adminmenu-quickfind-m' + l);
          $('.dqx_adminmenu-quickfind-parent-m' + l, $divRoot).removeClass('dqx_adminmenu-quickfind-parent-m' + l);
        }
        if (str.length) {
          // highlight matching element
          var prefix = str.substr(0, 3);
          // clean up all classes
          var l = prefix.length;
          if (searchindex[l][prefix]) {
            var candidates = searchindex[l][prefix];
            var matches = [];
            for (var i=0; i<candidates.length; ++i) {
              var txt = $(candidates[i]).html().toLowerCase();
              if (txt.split(str).length > 1) {
                // match !
                $(candidates[i]).addClass('dqx_adminmenu-quickfind-m' + l);
                $(candidates[i]).parents('li', 'tr').each(
                  function(){
                    $(this).addClass('dqx_adminmenu-quickfind-parent-m' + l);
                  }
                );
                matches.push(txt);
              }
            }
          }
        }
      }
    };
  };


  d.quickfind.buildSearchIndex = function($divRoot) {
    var searchindex = [null, {}, {}, {}];
    var searchindex_strings = [null, {}, {}, {}];
    $('a', $divRoot).each(
      function(){
        var txt = $(this).html().toLowerCase();
        var words = txt.split(' ');
        for (var i = 0; i < words.length; ++i) {
          if (words[i].length) {
            for (var j = 1; j < words[i].length && j < 3; ++j) {
              var prefix = words[i].substr(0, j);
              if (!searchindex[j][prefix]) {
                searchindex[j][prefix] = [];
                searchindex_strings[j][prefix] = [];
              }
              if (i == 0 || j > 1) {
                searchindex[j][prefix].push(this);
                searchindex_strings[j][prefix].push(txt);
              }
            }
            for (var j = 0; j + 2 < words[i].length; ++j) {
              var substr = words[i].substr(j, 3);
              if (!searchindex[3][substr]) {
                searchindex[3][substr] = [];
                searchindex_strings[3][substr] = [];
              }
              searchindex[3][substr].push(this);
              searchindex_strings[3][substr].push(txt);
            }
          }
        }
      }
    );
    return new d.quickfind.SearchIndex(searchindex, $divRoot);
  };


  d.quickfind.buildSearchWidget = function($divRoot) {

    var observers = [];

    var str = '';
    var $searchmon = $('<span id="dqx_adminmenu-quickfind-monitor">');
    var $searchmon_box = $('<div>&nbsp;</div>');
    var $searchfield = $('<input size="6" id="dqx_adminmenu-quickfind"/>');
    var $searchfield_li = $('<li id="dqx_adminmenu-quickfind-li">');
    var $searchmon_li = $('<li id="dqx_adminmenu-quickfind-monitor-li">');
    $('#dqx_adminmenu-admin', $divRoot).append($searchmon_li).append($searchfield_li);
    $searchfield_li.append($searchfield);
    $searchmon_li.append($searchmon_box);
    $searchmon_box.prepend($searchmon);

    var $focused;
    $('input, window').focus(
      function(){
        if (this != $searchfield[0]) {
          $focused = $(this);
        }
      }
    );

    $('#dqx_adminmenu').hover(
      function(){
        $searchfield.focus();
        $searchmon_li.addClass('dqx_adminmenu-quickfind-focus');
      },
      function(){
        str = '';
        $searchfield.val('');
        $searchmon.html('');
        $searchmon_li.removeClass('dqx_adminmenu-quickfind-focus');
        if ($focused) {
          $focused.focus();
        }
        else {
          $searchfield.blur();
        }
      }
    );

    $searchfield.keydown(
      function(event){
        var c = String.fromCharCode(event.keyCode).toLowerCase();
        // use backspace to delete the last char
        if (event.keyCode == 8) {
          str = str.slice(0, -1);
        }
        else {
          str += c;
        }
        // use space to clear the search field.
        var fragments = str.split(' ');
        if (fragments.length > 1) {
          str = fragments[fragments.length - 1];
        }
        $searchmon.html(str);
        for (var i = 0; i < observers.length; ++i) {
          observers[i].setSearchString(str);
        }
      }
    );

    return {
      observeSearchString: function(observer) {
        observers.push(observer);
      }
    };
  };


  /**
   * Performance:
   * This was measured to take 225 millis.
   * (depends on client machine, and size of the menu)
   * Reason is, every link has to be inspected separately.
   */
  d.hooks.init_latest.quickfind = function(context, settings, $divRoot) {
    var widget = d.quickfind.buildSearchWidget($divRoot);
    widget.observeSearchString(d.quickfind.buildSearchIndex($divRoot));
  };

})();
