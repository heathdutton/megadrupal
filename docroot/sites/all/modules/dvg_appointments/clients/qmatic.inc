<?php

class AppointmentsQmaticClient extends AppointmentsClientApi {
  protected $timezone = 'UTC';
  protected $api_method= 'soap';

  const APPOINTMENT_STATUS_BOOKED          = 1;
  const APPOINTMENT_STATUS_INVALID_PRODUCT = 2;
  const APPOINTMENT_STATUS_INVALID_DATE    = 3;
  const APPOINTMENT_STATUS_INVALID_TIME    = 4;
  const APPOINTMENT_STATUS_UNKNOWN_ERROR   = 5;

  function __construct() {
    $link = variable_get('dvg_appointments_qmatic_url', '');
    if (empty($link)) {
      throw new Exception('No WSDL configured.');
    }

    $this->soap_client = new SoapClient($link, variable_get('dvg_appointments_soap_options', array()));
  }

  protected function hash_email($email) {
    return md5(strtolower($email));
  }

  public static function config_form() {
    $form = array();

    $form['dvg_appointments_qmatic_url'] = array(
      '#title' => t('Qmatic WSDL url'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_qmatic_url', ''),
      '#required' => TRUE,
    );

    return $form;
  }

  public function get_available_products() {
    $products = array();
    $available_products = $this->call('getAvailableProducts');
    foreach ($available_products->getAvailableProductsReturn as $product) {
      $products[$product->linkId] = array(
        'id' => $product->linkId,
        'name' => $product->name,
        'duration' => $product->appointLength,
      );
    }

    uasort($products, array($this, 'sort_available_products'));

    return $products;
  }

  protected function get_available_appointment_days($product_id) {
    $available_appointment_days = $this->call('getAvailableAppointmentDays', array(
      'productLinkID' => $product_id,
    ));

    return $available_appointment_days->getAvailableAppointmentDaysReturn;
  }

  protected function get_available_appointment_times($product_id, $date) {
    $available_appointment_times = $this->call('getAvailableAppointmentTimes', array(
      'productLinkID' => $product_id,
      'appDate' => $date,
    ));

    return $available_appointment_times->getAvailableAppointmentTimesReturn;
  }

  public function get_customer_appointments($vars) {
    $customer_id = $this->hash_email($vars['email']);
    $now = new DateTime();
    $now = $now->format('Y-m-d\TH:i:s\Z');
    $get_customer_appointment = $this->call('getCustomerAppointments', array(
      'customerId' => $customer_id,
      'fromDate' => $now,
    ));

    if (isset($get_customer_appointment->getCustomerAppointmentsReturn)) {
      $appointments = $get_customer_appointment->getCustomerAppointmentsReturn;
      if (!is_array($appointments)) {
        $appointments = array($appointments);
      }
      foreach ($appointments as $appointment) {
        if ($appointment->appointmentId == $vars['appointment_id']) {
          return $appointment;
        }
      }
    }

    return FALSE;
  }

  public function delete_appointment($appointment_id) {
    $delete_appointment = $this->call('deleteAppointment', array(
      'appointmentId' => $appointment_id,
    ));

    return $delete_appointment->deleteAppointmentReturn;
  }

  public function book_appointment($appointment_data, $book_data) {
    $date = dvg_appointments_format_date($appointment_data['datetime'], 'custom', 'Y-m-d') . 'T12:00:00.000Z';
    $time = '1970-01-01T' . dvg_appointments_format_date($appointment_data['datetime'], 'custom', 'H:i:s') . '.000Z';

    if (!isset($book_data['piNo'])) {
      if (isset($book_data['email'])) {
        $book_data['piNo'] = $this->hash_email($book_data['email']);
      }
      else {
        watchdog('dvg_appointments', 'Booking failed: invalid piNo: @book_data', array('@book_data' => var_export($book_data, 1)), WATCHDOG_ERROR);
        throw new Exception('Booking failed. No piNo and/or email found.', DVG_APPOINTMENTS_ERRORCODE_UNKNOWN);
      }
    }

    $appointment_data = array(
      'productLinkID' => (isset($appointment_data['id'])) ? $appointment_data['id'] : '',
      'appDate' => $date,
      'appTime' => $time,
      'name' => (isset($book_data['name'])) ? $book_data['name'] : '',
      'address' => (isset($book_data['address'])) ? $book_data['address'] : '',
      'town' => (isset($book_data['town'])) ? $book_data['town'] : '',
      'state' => (isset($book_data['state'])) ? $book_data['state'] : '',
      'zipCode' => (isset($book_data['zipCode'])) ? $book_data['zipCode'] : '',
      'country' => (isset($book_data['country'])) ? $book_data['country'] : '',
      'phone' => (isset($book_data['phone'])) ? $book_data['phone'] : '',
      'phone2' => (isset($book_data['phone2'])) ? $book_data['phone2'] : '',
      'email' => (isset($book_data['email'])) ? strtolower($book_data['email']) : '',
      'piNo' => $book_data['piNo'],
      'accountNo' => (isset($book_data['accountNo'])) ? $book_data['accountNo'] : '',
      'cardNum' => (isset($book_data['cardNum'])) ? $book_data['cardNum'] : '',
      'title' => (isset($book_data['title'])) ? $book_data['title'] : '',
      'born' => (isset($book_data['born'])) ? $book_data['born'] : '',
      'vipLevel' => (isset($book_data['vipLevel'])) ? $book_data['vipLevel'] : '',
      'gender' => (isset($book_data['gender'])) ? $book_data['gender'] : '',
      'social' => (isset($book_data['social'])) ? $book_data['social'] : '',
      'family' => (isset($book_data['family'])) ? $book_data['family'] : '',
      'customerClass' => (isset($book_data['customerClass'])) ? $book_data['customerClass'] : '',
      'comments' => (isset($book_data['comments'])) ? $book_data['comments'] : '',
    );

    $book_appointment = $this->call('bookAppointment', $appointment_data);
    $return = $book_appointment->bookAppointmentReturn;

    if (empty($return->appointmentId) || $return->appointmentStatus != self::APPOINTMENT_STATUS_BOOKED) {
      $date_time_error = in_array($return->appointmentStatus, array(self::APPOINTMENT_STATUS_INVALID_DATE, self::APPOINTMENT_STATUS_INVALID_TIME));
      $error_code = ($date_time_error) ? DVG_APPOINTMENTS_ERRORCODE_DATE_TIME : DVG_APPOINTMENTS_ERRORCODE_UNKNOWN;
      watchdog('dvg_appointments', 'Booking failed with code @code', array('@code' => $return->appointmentStatus), WATCHDOG_ERROR);
      throw new Exception('Booking failed. No appointment id.', $error_code);
    }

    return array(
      'id' => $return->appointmentId,
      'status' => $return->appointmentStatus,
    );
  }

  public function get_dates_times($product_ids, $durations, $persons) {
    $dates_all = array();
    foreach ($product_ids as $product_link_id) {
      foreach ($this->get_available_appointment_days($product_link_id) as $date) {
        $date = str_replace('12:00:00.000Z', '00:00:00.000Z', $date);
        $time = $this->get_available_appointment_times($product_link_id, $date);
        if (!isset($dates_all[$product_link_id])) {
          $dates_all[$product_link_id] = array();
          $dates_all[$product_link_id]['dates'] = array();
          $dates_all[$product_link_id]['product_id'] = $product_link_id;
          $dates_all[$product_link_id]['duration'] = $durations[$product_link_id];
        }
        if (!isset($dates_all[$product_link_id]['dates'][$date])) {
          $dates_all[$product_link_id]['dates'][$date] = is_array($time) ? $time : array($time);
        }
      }
    }

    // _appointments_magic checks the dates.
    $right_dates_time = $this->_appointments_magic($dates_all, $persons);

    return $right_dates_time;
  }

  /**
   * Magic function that check all correct time for appointments.
   */
  protected function _appointments_magic($dates_all, $persons = array()) {
    $available_dates = $this->check_dates($dates_all);

    $good_times = array();
    $dates = array_shift($dates_all);
    foreach ($available_dates as $date) {
      foreach ($dates['dates'][$date] as $time) {
        $person_is_ok = array();
        $appointments = array();
        for ($i = 0; $i < $persons[$dates['product_id']]; $i++) {
          // Why was ($i+1) ? Deleted because 9;00 was not visible
          $search_time = strtotime($time) + $dates['duration'] * 60 * ($i);
          foreach ($dates['dates'][$date] as $_time) {
            if ($search_time == strtotime($_time)) {
              $person_is_ok[$i] = TRUE;
              $appointments[$_time] = array(
                'product_id' => $dates['product_id'],
                'duration' => $dates['duration'],
                'date' => $date,
                'time' => $_time,
              );
            }
          }
        }

        // Is this ok? Then check for person 1
        $is_ok = TRUE;
        for ($p = 0; $p < $persons[$dates['product_id']]; $p++) {
          if (!isset($person_is_ok[$p])) {
            $is_ok = FALSE;
          }
        }

        if ($is_ok) {
          $good_times[] = array_values($appointments);
        }
      }
    }

    foreach ($dates_all as $dates) {
      $good_times_temp = $good_times;
      $good_times = array();
      foreach ($available_dates as $date) {
        foreach ($dates['dates'][$date] as $time) {
          foreach ($good_times_temp as $appointments) {
            $last_appointment = end($appointments);

            if ($date == $last_appointment['date'] && (strtotime($time) == strtotime($last_appointment['time']) + $last_appointment['duration'] * 60)) {
              $person_is_ok = array();
              for ($i = 1; $i <= $persons[$dates['product_id']]; $i++) {
                // This part should check the next moment that is available. The for-loop should start.
                $search_time = strtotime($last_appointment['time']) + $last_appointment['duration'] * 60 * ($i);
                foreach ($dates['dates'][$date] as $_time) {
                  if ($search_time == strtotime($_time)) {
                    $person_is_ok[$i] = TRUE;
                    $appointments[] = array(
                      'product_id' => $dates['product_id'],
                      'duration' => $dates['duration'],
                      'date' => $date,
                      'time' => $_time,
                    );
                  }
                }
              }

              // Is this OK? Then start with 1 because that should be added ($i) to the time in the above loop to find the next moment.
              $is_ok = TRUE;
              for ($o = 1; $o <= $persons[$dates['product_id']]; $o++) {
                if (!isset($person_is_ok[$o])) {
                  $is_ok = FALSE;
                }
              }

              if ($is_ok) {
                $good_times[] = $appointments;
              }
            }
          }
        }
      }
    }

    // Returns all good times that need to be selectable.
    return $good_times;
  }

  /**
   * Helper function which check the same date for appointments.
   */
  protected function check_dates($dates_all) {
    $right_dates = array();

    $first_dates = array_shift($dates_all);
    foreach ($first_dates['dates'] as $date => $time) {
      $right_dates[] = $date;
    }

    foreach ($dates_all as $product) {
      $right_dates_temp = $right_dates;
      $right_dates = array();
      foreach ($product['dates'] as $date => $time) {
        if (in_array($date, $right_dates_temp)) {
          $right_dates[] = $date;
        }
      }
    }

    return $right_dates;
  }

  public function get_book_fields() {
    return array(
      'name' => 'Name',
      'phone' => 'Telephone',
      'email' => 'Email',
      'born' => 'Birthdate',
    );
  }

  protected function get_cache_expire() {
    return array(
      'getAvailableAppointmentTimes' => 10,
      'getAvailableProducts' => 60 * 60 * 24,
    );
  }
}
