<?php

$rule = '{ "rules_once_per_week" : {
    "LABEL" : "Example once per week",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "REQUIRES" : [ "rules_onceperday", "rules" ],
    "ON" : { "once_per_day" : [] },
    "IF" : [ { "rules_onceperday_condition_dayofweek" : { "weekday" : "1" } } ],
    "DO" : [
      { "mail" : {
          "to" : "mail@example.com",
          "subject" : "It is a Monday!",
          "message" : "Once a week message sent [site:current-date]",
          "language" : [ "" ]
        }
      }
    ]
  }
}';
