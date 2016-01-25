var SmartDates = SmartDates || {};


SmartDates.register_dates = function(date1, date2, format) {
    var date1start = Date.parseExact(date1.val(), format);

    date1.change(function() {
        var date2now = Date.parseExact(date2.val(), format);
        var date1now = Date.parseExact(date1.val(), format);

        var interval = date2now.valueOf() - date1start.valueOf();
        // alert("Date1start: " + date1start + "\nDate2now: " + date2now +  "\nDate1now: " + date1now + "\nInterval: " + interval);
        date2.val(date1now.clone().addMilliseconds(interval).toString(format));
        date1start = Date.parseExact(date1.val(), format);
    });
};

SmartDates.register_times = function(time1, time2, format) {
    var time1start = Date.parseExact(time1.val(), format);

    time1.change(function() {
        var time1now = Date.parseExact(time1.val(), format);
        var time2now = Date.parseExact(time2.val(), format);

        // alert("Time1start: " + time1start + "\nTime2now: " + time2now +  "\nTime1now: " + time1now + "\nInterval: " + interval);

        if (time2now) {
            // if we started at the same point
            if (time2now.valueOf() == time1start.valueOf()) {
                time2.val(time1now.clone().addHours(2).toString(format));
            }
            else {
                var interval = time2now.valueOf() - time1start.valueOf();
                time2.val(time1now.clone().addMilliseconds(interval).toString(format));
            }
        } else {
            time2.val(time1now.clone().addHours(2).toString(format));
        }
        time1start = Date.parseExact(time1.val(), format);

    });
}
