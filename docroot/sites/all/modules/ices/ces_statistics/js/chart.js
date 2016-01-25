/**
* @file
* Js script to produce the graphics in statistics page.
*/

(function ($) {
Drupal.behaviors.ces_statistics = {
attach: function(context, settings) {

  var usersobject = Drupal.settings.ces_statistics.staticsusers;
  var activityobject = Drupal.settings.ces_statistics.staticsactivity;
  var transobject = Drupal.settings.ces_statistics.staticstrans;
  var usersarray = new Array();
  var activityarray = new Array();
  var activityarray2 = new Array();
  var amountarray = new Array();
  var numberarray = new Array();
  var maxmin01 = new Array();
  var maxmin02 = new Array();

  $.each(usersobject, function (index, order) {
    usersarray.push([order.usersdate,parseInt(order.usersnumber)]);
  });

  $.each(activityobject, function (index, order) {
    activityarray.push([order.activitylevel,parseInt(order.activitypercent)]);
    activityarray2.push([parseInt(order.activitypercent) + '%(' + order.activitynumber + ')']);
  });

  $.each(transobject, function (index, order) {
    amountarray.push([order.transdate,parseFloat(order.transamount).toFixed(0),parseFloat(order.transamount).toFixed(2)]);
    numberarray.push([order.transdate,order.transnumber]);
    maxmin01.push(order.transnumber);
    maxmin02.push(order.transamount);
  });

  // interval in x axis
  var usersint = '1 month';
  if (usersarray.length > 18) {
    var usersint = '2 month';
    if (usersarray.length > 30) {
      var usersint = '3 month';
    };
  };

  var transint = '1 month';
  if (numberarray.length > 18) {
    var transint = '2 month';
    if (numberarray.length > 24) {
      var transint = '3 month';
    };
  };

  // Calculate max in y axis and protect against max=0
  var maxnumber = parseFloat(Math.max.apply(Math, maxmin01) * 1.2).toFixed(0);
  if (maxnumber == 0) {
    maxnumber = 10;
  }
  var maxamount = parseFloat(Math.max.apply(Math, maxmin02) * 1.2).toFixed(0);
  if (maxamount == 0) {
    maxamount = 10;
  }

  // Number of accounts chart
  var plot1 = $.jqplot('chartdiv1', [usersarray], {
    axes:{
      xaxis:{
        renderer:$.jqplot.DateAxisRenderer,
    tickRenderer: $.jqplot.CanvasAxisTickRenderer,
    tickOptions:{angle: 30,formatString:'%b-%y'},
    min:usersarray[0][0],
    tickInterval:usersint,
      },
    yaxis:{
      autoscale:true,
      min: 0,
    },
    },
    series:[{
      xaxis:'xaxis',
    yaxis:'yaxis',
    color: "#66AA00",
    rendererOptions: {smooth: true},
    pointLabels:{show:true, stackedValue: false},
    }],
    legend:{
      show: false,
    }
  });

  // Last year accounts' activity chart
  var plot2 = $.jqplot ('chartdiv2', [activityarray], {
    seriesColors: ["#eeffaa", "#bbee55", "#aadd44", "#99cc33", "#88bb22"],
    seriesDefaults: {
      renderer: jQuery.jqplot.PieRenderer,
    rendererOptions: {showDataLabels: true, dataLabels: activityarray2, startAngle: 270}
    },
    legend: {show:true, location: 'e'},
  });

  // Number of transactions chart
  var plot3 = $.jqplot('chartdiv3', [numberarray], {
    axes:{
      xaxis:{
        renderer:$.jqplot.DateAxisRenderer,
    tickRenderer: $.jqplot.CanvasAxisTickRenderer,
    tickOptions:{angle: 30,formatString:'%b-%y'},
    min:usersarray[0][0],
    tickInterval:usersint,
      },
    yaxis:{
      min: 0,
      max:maxnumber,
    },
    },
    series:[{
      xaxis:'xaxis',
    yaxis:'yaxis',
    color: "#66AA00",
    rendererOptions: {smooth: true},
    pointLabels:{show:true, stackedValue: false},
    }],
    legend:{
      show: false,
    }
  });

  // Amount of transactions chart
  var plot4 = $.jqplot('chartdiv4', [amountarray], {
    axes:{
      xaxis:{
        renderer:$.jqplot.DateAxisRenderer,
        tickRenderer: $.jqplot.CanvasAxisTickRenderer,
        tickOptions:{angle: 30,formatString:'%b-%y'},
        min:amountarray[0][0],
        tickInterval:transint,
      },
      yaxis:{
        min:0,
        max:maxamount,
      },
    },

    seriesDefaults: {
      pointLabels:{show:true, stackedValue: false},
    },

    series:[{
      renderer:$.jqplot.BarRenderer,
      rendererOptions: {shadowAlpha: 0, barWidth: 20, barPadding: 20, barDirection: 'vertical'},
      xaxis:'xaxis',
      color: "#99CC33",
    }],
  });

//  $(window).resize(function() {
//    plot1.replot( { resetAxes: true } );
//    plot2.replot( { resetAxes: true } );
//    plot3.replot( { resetAxes: true } );
//    plot4.replot( { resetAxes: true } );
//  });

}
};
})(jQuery);
