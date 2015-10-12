<div class="abs-biblesearch-browsethebible">
  <div class="browsetitle"><h3>Browse the Bible</h3></div>
  <div class="breadcrumbs"></div>
  <div id="browsemessage" style="display: none;">Please wait...</div>
  <div id="browseversion">
    <p><strong>First Choose a Version:</strong></p>
    <?php
    foreach ($versions as $version) {
      echo '<a class="version" href="#" versionid="' . $version->id . '">(' . $version->id . ') ' . $version->name . '</a>';
    }
    ?>
  </div>
  <div id="browsetestament" style="display: none;">
    <p><strong>Now Choose a Group:</strong></p>
    <a class="testament" href="#" testament="OT">Old Testament</a>
    <a class="testament" href="#" testament="NT">New Testament</a>
  </div>
  <div id="browsebook"></div>
  <div id="browsechapter"></div>
</div>

<script type="text/javascript">
(function ($) { // drupal 7 jquery wrapper

  var browseversion = '';
  var browsetestament = '';
  var browsebook = '';

  function absBrowseSetBreadcrumbs() {
    var content = '';
    if ( browseversion != '' ) {
      content += 'Your choices: <span ';
      if ( browsetestament == '' ) { content += 'class="active" '; }
      content += 'goto="version">' + browseversion + '</span>';
      }

    if ( browsetestament != '' ) {
      content += ' &raquo; <span ';
      if ( browsebook == '' ) { content += 'class="active" '; }
      content += 'goto="testament">' + browsetestament + '</span>';
      }

    if ( browsebook != '' ) {
      content += ' &raquo; <span class="active" goto="book">' + browsebook + '</span>';
      }

    $('.abs-biblesearch-browsethebible .breadcrumbs').html(content);

    $('.abs-biblesearch-browsethebible .breadcrumbs span').click( function() {
      switch ( $(this).attr('goto') ) {
        case 'version':
          browseversion = ''; browsetestament = ''; browsebook = '';
          absBrowseSetBreadcrumbs();

          $('#browsemessage').hide();
          $('#browsetestament').hide();
          $('#browsebook').hide();
          $('#browsechapter').hide();
          $('#browseversion').fadeIn();

          break;

        case 'testament':
          browsetestament = ''; browsebook = '';
          absBrowseSetBreadcrumbs();

          $('#browsemessage').hide();
          $('#browseversion').hide();
          $('#browsebook').hide();
          $('#browsechapter').hide();
          $('#browsetestament').fadeIn();

          break;

        case 'book':
          browsebook = '';
          absBrowseSetBreadcrumbs();

          $('#browsemessage').hide();
          $('#browseversion').hide();
          $('#browsetestament').hide();
          $('#browsechapter').hide();
          $('#browsebook').fadeIn();

          break;
      }
    });
  }

  function absBrowseVersionClicked( clicked ) {
    browseversion = clicked.attr('versionid');
    absBrowseSetBreadcrumbs();

    $('#browseversion').hide();
    $('#browsebook').hide();
    $('#browsechapter').hide();

    $('#browsetestament').fadeIn();
  }

  function absBrowseTestamentClicked( clicked ) {
    browsetestament = clicked.attr('testament');
    absBrowseSetBreadcrumbs();

    $('#browseversion').hide();
    $('#browsetestament').hide();
    $('#browsechapter').hide();

    $('#browsebook').html('');
    $('#browsemessage').show();

    $.get('/abs_biblesearch/ajax/versionbooksinatestament/'+browseversion+'/'+browsetestament, function(data) {
      // Why double check here.  Because a person could click a breadcrumb WHILE this ajax is
      // in progress.  If so this ajax response should be ignored.
      if ( browseversion != '' && browsetestament != '' ) {
        $('#browsemessage').hide();
        $('#browsebook').html(data
            .replace( new RegExp( "\\n", "g" ), '')  // ... but we've got javascript returned, which needs removing.
            .replace( /<script>.*?<noscript>/g, '' ) // Make sure to leave in the FUMS images.
            .replace( /<\/noscript>/g, '' )
          ).fadeIn();

        $('.abs-biblesearch-browsethebible a.book').click( function() {
          absBrowseBookClicked ( $(this) );
        });
      }

        });
  }

  function absBrowseBookClicked( clicked ) {
    browsebook = clicked.attr('bookid');
    absBrowseSetBreadcrumbs();

    $('#browseversion').hide();
    $('#browsetestament').hide();
    $('#browsebook').hide();
    $('#browsemessage').show();
    $('#browsechapter').html('');

    $.get('/abs_biblesearch/ajax/versionchaptersinabook/'+browseversion+'/'+browsebook, function(data) {
      if ( browseversion != '' && browsebook != '' ) {
        $('#browsemessage').hide();
        $('#browsechapter').html(data
            .replace( new RegExp( "\\n", "g" ), '')  // ... but we've got javascript returned, which needs removing.
            .replace( /<script>.*?<noscript>/g, '' ) // Make sure to leave in the FUMS images.
            .replace( /<\/noscript>/g, '' )
          ).fadeIn();
      }
    });
  }

  $('.abs-biblesearch-browsethebible a.version').click( function() {
    absBrowseVersionClicked( $(this) );
  });

  $('.abs-biblesearch-browsethebible a.testament').click( function() {
    absBrowseTestamentClicked( $(this) );
  });

})(jQuery);
</script>