// $Id$

/**
 * @file
 * For displaying Pie Chart and Bar Graph in Cloud Dashboard module
 *
 * Copyright (c) 2010-2011 DOCOMO Communications Laboratories USA, Inc.
 * 
 */
Drupal.behaviors.cloudDashboard = function (context) {
  
  $('#cloud-dashboard-all-cloud-data table div').click(
    function() {
      
      //Hide every popup
      $('.cloud_item_container').each(
        function() {
          $(this).css('display', 'none');
        }
      )
      
      //Get the class of the current item
      var currentId = '#'+$(this).attr('class');
      
      //Display the current item
      $(currentId).css('display','block');
      
      // to position ID, we need the window height/width      
      var msg = $(currentId);
      var height = $(window).height();
      var width = $(document).width();
  
      msg.css({
          'left' : width/2 - (msg.width() / 2),  // half width - half element width
          'top' : height/2 - (msg.height() / 2), // similar
          'z-index' : 999                        // make sure element is on top
      });
    }
  )
  
  $('.cloud_popup_block_title span').click(
    function() {
      //Hide the popup
      $('.cloud_item_container').css('display','none');
    }
  )
  
  // The Pie-chart plugin
  if ($("#dashboard-usage-charts-pie-chart").length > 0 && Drupal.settings.cloudDashboardPieData) {
    Raphael.fn.cloudDashboardPieChart = function (cx, cy, r, values, labels, stroke) {
      var paper = this,
        rad = Math.PI / 180,
        chart = this.set();
      
        var sectorCount = 0;
        r = 115;
      function sector(cx, cy, r, startAngle, endAngle, params) {
        if (sectorCount <= 1) {
          // Only one sector, which means we need to draw a circle
          return paper.circle(cx, cy, r).attr(params);
        }
        else {
          var x1 = cx + r * Math.cos(-startAngle * rad),
            x2 = cx + r * Math.cos(-endAngle * rad),
            y1 = cy + r * Math.sin(-startAngle * rad),
            y2 = cy + r * Math.sin(-endAngle * rad);
          return paper.path(["M", cx, cy, "L", x1, y1, "A", r, r, 0, +(endAngle - startAngle > 180), 0, x2, y2, "z"]).attr(params);
        }
      }
    
      var angle = 90,
      total = 0,
      start = 0,
      process = function (j) {
      var value = values[j],
          angleplus = 360 * value / total,
          popangle = angle + (angleplus / 2),
          color = "hsb(" + start + ", 1, .5)",
          ms = 500,
          delta = 30,
          bcolor = "hsb(" + start + ", 1, 1)",
          p = sector(cx, cy, r, angle, angle + angleplus, {gradient: "90-" + bcolor + "-" + color, stroke: stroke, "stroke-width": 1, "cursor": "pointer"}),
          label = Drupal.t("@cloud | Cost: $@cost", {"@cloud": labels[j], "@cost": thousandSeparator(value.toFixed(2), ',')});
          
          if (sectorCount > 1) {
            
            var txt = paper.text(20, 10, label).attr({fill: "#494949", stroke: "none", opacity: 0, "font-family": "Fontin-Sans, Arial", "font-size": "14px", "font-weight": "bold", "text-anchor": "start"});
            var txt_dot = paper.text(10, 10, "|").attr({fill: bcolor, stroke: "none", opacity: 0, "font-family": 'Fontin-Sans, Arial', "font-size": "30px", "font-weight": "bold", "text-anchor": "left"});
          }
          else {
            // For only one sector we put the text at the top
            var txt = paper.text(20, 10, label).attr({fill: "#494949", stroke: "none", opacity: 0, "font-family": 'Fontin-Sans, Arial', "font-size": "14px", "font-weight": "bold", "text-anchor": "start"});
            var txt_dot = paper.text(10, 10, "|").attr({fill: bcolor, stroke: "none", opacity: 0, "font-family": 'Fontin-Sans, Arial', "font-size": "30px", "font-weight": "bold", "text-anchor": "left"});
          }
          

      p.mouseover(function () {
        p.animate({scale: [1.1, 1.1, cx, cy]}, ms, "elastic");
        txt.animate({opacity: 1}, ms, "elastic");
        txt_dot.animate({opacity: 1}, ms, "elastic");
        }).mouseout(function () {
          p.animate({scale: [1, 1, cx, cy]}, ms, "elastic");
          txt.animate({opacity: 0}, ms);
          txt_dot.animate({opacity: 0}, ms);
        });
        angle += angleplus;
        chart.push(p);
        chart.push(txt);
        chart.push(txt_dot);
        start += .1;
      };
    
      for (var i = 0, ii = values.length; i < ii; i++) {
        total += values[i];
        sectorCount++;
      }
      
      for (var i = 0; i < ii; i++) {
        process(i);
      }
    
      return chart;
    };
  
    // Grid plugin
    Raphael.fn.drawGrid = function (x, y, w, h, wv, hv, color) {
    
      color = color || "#000";
      var path = ["M", Math.round(x) + .5, Math.round(y) + .5, "L", Math.round(x + w) + .5, Math.round(y) + .5, Math.round(x + w) + .5, Math.round(y + h) + .5, Math.round(x) + .5, Math.round(y + h) + .5, Math.round(x) + .5, Math.round(y) + .5],
        rowHeight = h / hv,
        columnWidth = w / wv;
      for (var i = 1; i < hv; i++) {
        path = path.concat(["M", Math.round(x) + .5, Math.round(y + i * rowHeight) + .5, "H", Math.round(x + w) + .5]);
      }
      //for (i = 1; i < wv; i++) {
      //  path = path.concat(["M", Math.round(x + i * columnWidth) + .5, Math.round(y) + .5, "V", Math.round(y + h) + .5]);
      //}
      return this.path(path.join(",")).attr({stroke: color});
    };
  
  // Render the pie-chart
  //if ($("#dashboard-usage-charts-pie-chart").length > 0 && Drupal.settings.cloudDashboardPieData) {
    var data = Drupal.settings.cloudDashboardPieData.data,
      labels = Drupal.settings.cloudDashboardPieData.labels,
      container = $("#dashboard-usage-charts-pie-chart"),
      width = container.width(),
      height = container.height();
      c_x = (width / 2) - 60;
      c_y = (height / 2);
    Raphael("dashboard-usage-charts-pie-chart", width, height).cloudDashboardPieChart(c_x, c_y, 80, data, labels, "#fff");    
  }
  //Hack for IE to fix the bar chart title
  if (jQuery.browser.msie) {
    if ($('.dashboard-usage-charts-bar .bar-chart-y-axis-label').size()) {
      $('.dashboard-usage-charts-bar .bar-chart-y-axis-label').css('z-index', 100);
      $('.without-pie-chart #dashboard_usage_charts-bar-data').css('margin-left', '25px')
    }
  }
  
  
  // Render the bar-chart
  if ($("#dashboard_usage_charts-bar-data").length > 0 && Drupal.settings.cloudDashboardBarData) {
    
    var data = Drupal.settings.cloudDashboardBarData.data,
      labels = Drupal.settings.cloudDashboardBarData.labels;
    
    
    //alert(data);
    new Ico.BarGraph(document.getElementById("dashboard_usage_charts-bar-data"),
      {cost: data},
      {
    one:                      12,
        labels:                   labels,
        colours:                  {cost: "#338CA2"},
        hover_color :             "#006677",
        datalabels :              {cost: data},
    background_colour:        '#fff',
        grid:                     true,
    width:                    420
      }
    );
    
    //new Ico.BarGraph($('bargraph_9'),  {shoe_size: [1, 1, 1, 0, 2, 4, 6, 8, 3, 9, 6]}, { colours: {shoe_size: '#990000' }, grid: true });

  }
  
  function thousandSeparator(n,sep) {
      var sRegExp = new RegExp('(-?[0-9]+)([0-9]{3})'),
      sValue=n+'';

      if (sep === undefined) {sep=',';}
      while(sRegExp.test(sValue)) {
      sValue = sValue.replace(sRegExp, '$1'+sep+'$2');
      }
      return sValue;
  }
}