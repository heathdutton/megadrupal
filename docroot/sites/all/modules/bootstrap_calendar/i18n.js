if(!window.calendar_languages) {
	window.calendar_languages = {};
}
window.calendar_languages['i18n'] = {
		error_noview:     Drupal.t('Calendar: View {0} not found'),
		error_dateformat: Drupal.t('Calendar: Wrong date format {0}. Should be either "now" or "yyyy-mm-dd"'),
		error_loadurl:    Drupal.t('Calendar: Event URL is not set'),
		error_where:      Drupal.t('Calendar: Wrong navigation direction {0}. Can be only "next" or "prev" or "today"'),
		error_timedevide: Drupal.t('Calendar: Time split parameter should divide 60 without decimals. Something like 10, 15, 30'),

		no_events_in_day: Drupal.t('No events in this day.'),

		title_year:  Drupal.t('{0}'),
		title_month: Drupal.t('{0} {1}'),
		title_week:  Drupal.t('week {0} of {1}'),
		title_day:   Drupal.t('{0} {1} {2}, {3}'),

		week:        Drupal.t('Week {0}'),
		all_day:     Drupal.t('All day'),
		time:        Drupal.t('Time'),
		events:      Drupal.t('Events'),
		before_time: Drupal.t('Ends before timeline'),
		after_time:  Drupal.t('Starts after timeline'),


		m0:  Drupal.t('January'),
		m1:  Drupal.t('February'),
		m2:  Drupal.t('March'),
		m3:  Drupal.t('April'),
		m4:  Drupal.t('May'),
		m5:  Drupal.t('June'),
		m6:  Drupal.t('July'),
		m7:  Drupal.t('August'),
		m8:  Drupal.t('September'),
		m9:  Drupal.t('October'),
		m10: Drupal.t('November'),
		m11: Drupal.t('December'),

		ms0:  Drupal.t('Jan'),
		ms1:  Drupal.t('Feb'),
		ms2:  Drupal.t('Mar'),
		ms3:  Drupal.t('Apr'),
		ms4:  Drupal.t('May'),
		ms5:  Drupal.t('Jun'),
		ms6:  Drupal.t('Jul'),
		ms7:  Drupal.t('Aug'),
		ms8:  Drupal.t('Sep'),
		ms9:  Drupal.t('Oct'),
		ms10: Drupal.t('Nov'),
		ms11: Drupal.t('Dec'),

		d0: Drupal.t('Sunday'),
		d1: Drupal.t('Monday'),
		d2: Drupal.t('Tuesday'),
		d3: Drupal.t('Wednesday'),
		d4: Drupal.t('Thursday'),
		d5: Drupal.t('Friday'),
		d6: Drupal.t('Saturday'),

	easter:       Drupal.t('Easter'),
	easterMonday: Drupal.t('Easter Monday'),

};
