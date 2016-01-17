
(function($) {

  $(document).ready( function() {
    if (Drupal.settings.pqp_settings.lang) {
      var pqp_baseUrl = Drupal.settings.basePath + Drupal.settings.pqp_settings.lang + '/';
    }
    else {
      var pqp_baseUrl = Drupal.settings.basePath;
    }
    $('a[href*="' + pqp_baseUrl + 'devel/pqp' + '"]').click( function(e) {
      e.preventDefault();
      $.get(pqp_baseUrl + '/devel/pqp', null, function(data) {
        window.location = location.href;
      })
    });
  });
  
  $(window).load( function () {
    if (Drupal.settings.pqp_settings.enable) {
      var $pqp_container = $('<div id="pqp-container" class="pQp hideDetails progress"></div>');
      var $pqp_pb = $('<div class="bar">&nbsp;</div>');

      if (Drupal.settings.pqp_settings.lang) {
        var pqp_baseUrl = Drupal.settings.basePath + Drupal.settings.pqp_settings.lang + '/';
      }
      else {
        var pqp_baseUrl = Drupal.settings.basePath;
      }

      $pqp_container.append($pqp_pb);
      $(document.body).append($pqp_container);
/* console.log(Drupal.settings.pqp_data); */
      if (Drupal.settings.pqp_data) {
        $.pqp.show($pqp_container, $pqp_pb, Drupal.settings.pqp_data);
      } else {
        $.post(pqp_baseUrl + 'pqp/get', {}, function(data) {
          $.pqp.show($pqp_container, $pqp_pb, data);
        });
      }
    }
  });

  $.pqp = {
    init: function() {
      PQP_DETAILS = false;
      PQP_HEIGHT = "short";

      var $tabs = $('#pqp-metrics .metric-tab');
      $tabs.each( function() {
        $tab = $(this);
        $tab.click( function(e) {
          var $tabname = $(this).attr('id').replace('pqp_','');
          e.preventDefault();
          $.pqp.changeTab($tabname);
          return false;
      	});
      });

      $('#pqp_toggleDetails').click( function(e) {
        e.preventDefault();
        $.pqp.toggleDetails();
        return false;
      });
      $('#pqp_toggleHeight').click( function(e) {
        e.preventDefault();
        $.pqp.toggleHeight();
        return false;
      });
      
      $('#pqp-queries .log-query .row:not(.noquery)').each( function() {
        $(this).data('explained',false);
        $(this).click( function() {
          var $row = $(this);
          if (!$row.data('explained')) {
            var $query = { sql: $(this).find('div').text() };
            var $explain = $('<div class="explain progress"><div class="bar">&nbsp;</div></div>');
            $row.append($explain);

            if (($query.sql.indexOf('SELECT') === 0) && ($query.sql.indexOf('SELECT COUNT') !== 0)) {
              $explain.animate({
                height: 120
              }, 'fast', 'linear');
              $.post(Drupal.settings.basePath + 'pqp/explain_query', $query, function(data) {
                $explain.append(data);
                $explain.css({ height: 'auto' }).removeClass('progress');
              });
            }
            else {
              var $s = $('<div class="messages error">EXPLAIN works on SELECT queries only.</div>');
              $explain.append($s).removeClass('progress');
            }

            $row.data('explained',true);
          }
        });
      });
    },

    show: function($pqp_container, $pqp_pb, data) {
      $pqp_pb.remove();
      $pqp_container.append(data).removeClass('progress');
      $("#pqp-container").fadeIn('normal');
      $.pqp.init();      
    },

    changeTab: function (tab) {
  		var $pQp = $('#pQp');
  		$.pqp.hideAllTabs();
  		$pQp.addClass(tab);
  	},

    hideAllTabs: function () {
  		var $pQp = $('#pQp');
  		$pQp.removeClass('console');
  		$pQp.removeClass('speed');
  		$pQp.removeClass('queries');
  		$pQp.removeClass('memory');
  		$pQp.removeClass('files');
  	},

    toggleDetails: function () {
  		var $container = $('#pqp-container');
  		var $metrics = $('#pqp-metrics');
  		var $footer = $('#pqp-footer');
  		
  		if(PQP_DETAILS){
        $metrics.css({
          float: 'left',
          width: '70%'
        }).find('.metric-tab').css('height', 20).find('h4').hide();
        $footer.find('.credit').after($metrics);
        $container.addClass('hideDetails');
        PQP_DETAILS = false;
  		}
  		else {
        $metrics.css({
          float: 'none',
          width: '100%'
        }).find('.metric-tab').css('height', 50).find('h4').fadeIn();
        $container.find('#pQp').prepend($metrics);
        $container.removeClass('hideDetails');
        if (!$container.hasClass('tallDetails')) $.pqp.toggleHeight();
        PQP_DETAILS = true;
  		}
  	},

    toggleHeight: function () {
  		var $container = $('#pqp-container');
  		
  		if(PQP_HEIGHT == "short"){
  			$container.addClass('tallDetails');
        var $th = $(window).height() - 150;
  			$('.pqp-box', $container).css('height', $th);
  			PQP_HEIGHT = "tall";
  		}
  		else{
  		  $container.removeClass('tallDetails');
  			$('.pqp-box', $container).css('height', 200);
  			PQP_HEIGHT = "short";
  		}
  	}

  };

})(jQuery);
