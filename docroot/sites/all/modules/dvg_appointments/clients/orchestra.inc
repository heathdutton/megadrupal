<?php

class AppointmentsOrchestraClient extends AppointmentsClientApi {
  protected $timezone = 'UTC';
  protected $api_method = 'rest';
  protected $branch_id;
  protected $url;

  function __construct() {
    $this->url = variable_get('dvg_appointments_orchestra_url', '');
    if (empty($this->url)) {
      throw new Exception('No REST url configured.');
    }
    $this->branch_id = variable_get('dvg_appointments_orchestra_branch_id');
    if (empty($this->branch_id)) {
      throw new Exception('No branch id configured.');
    }
  }

  protected function hash_email($email) {
    return md5(strtolower($email));
  }

  public static function config_form() {
    $form = array();

    $form['dvg_appointments_orchestra_url'] = array(
      '#title' => t('REST url'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_orchestra_url', ''),
      '#required' => TRUE,
    );
    $form['dvg_appointments_orchestra_branch_id'] = array(
      '#title' => t('Branch ID'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_orchestra_branch_id', ''),
      '#required' => TRUE,
    );
    return $form;
  }

  public function get_available_products() {
    $products = array();

    $available_products = $this->call('getAvailableProducts', array(
      'rest_url' => '/branches/' . $this->branch_id . '/services',
      'rest_method' => 'GET',
    ));

    foreach ($available_products->serviceList as $product) {
      $products[$product->publicId] = array(
        'id' => $product->publicId ,
        'name' => $product->name,
        'duration' => $product->duration,
        'additionalCustomerDuration' => $product->additionalCustomerDuration,
      );
    }

    uasort($products, array($this, 'sort_available_products'));

    return $products;
  }

  protected function get_available_appointment_days($product_id) {
    $available_appointment_days = $this->call('getAvailableAppointmentDays', array(
      'rest_url' => '/branches/' . $this->branch_id . '/services/' . $product_id . '/dates',
      'rest_method' => 'GET',
    ));
    foreach ($available_appointment_days->dates as &$date) {
      $date .= 'Z';
    }

    return $available_appointment_days->dates;
  }

  protected function get_available_appointment_times($product_id, $date) {
    $date = str_replace('T00:00:00Z', '', $date);
    $available_appointment_times = $this->call('getAvailableAppointmentTimes', array(
      'rest_url' => '/branches/' . $this->branch_id . '/services/' . $product_id . '/dates/' . $date . '/times',
      'rest_method' => 'GET',
    ));
    foreach ($available_appointment_times->times as &$time) {
      $time .= ':00.000Z';
    }
    return $available_appointment_times->times;
  }

  public function get_customer_appointments($vars) {
    $get_customer_appointment = $this->call('getGovAppointmentDetails', array(
      'rest_url' => '/appointments/' . $vars['appointment_id'],
      'rest_method' => 'GET',
    ), (isset($vars['reset']) ? $vars['reset'] : FALSE));
    if (isset($get_customer_appointment->appointment->services[0]->publicId) && strtolower($get_customer_appointment->appointment->customers[0]->email) == strtolower($vars['email'])) {
      $products = $this->get_available_products();
      $return = array(
        'appointmentId' => $vars['appointment_id'],
        'appointmentDate' => $get_customer_appointment->appointment->start,
        'appointmentTime' => $get_customer_appointment->appointment->start,
        'appointmentLength' => $products[$get_customer_appointment->appointment->services[0]->publicId]['duration'],
        'productName' => $products[$get_customer_appointment->appointment->services[0]->publicId]['name'],
      );
      return $return;
    }

    return FALSE;
  }

  public function delete_appointment(DvgAppointment $appointment) {
    $success = TRUE;
    foreach ($appointment->getRemoteAppointmentIds() as $remote_id) {
      $delete_appointment = $this->call('deleteAppointment', array(
        'rest_url' => '/appointments/' . $remote_id,
        'rest_method' => 'DELETE',
      ));
      if (isset($delete_appointment->msg)) {
        watchdog('dvg_appointments', 'Delete appointment failed with code @code', array('@code' => $delete_appointment->msg), WATCHDOG_ERROR);
      }
      $success = $success && !isset($delete_appointment->msg);
    }

    return $success;
  }

  public function book_appointment($appointment_data, $book_data) {
    if (isset($book_data['email'])) {
      $book_data['externalId'] = $this->hash_email($book_data['email']);
    }
    else {
      watchdog('dvg_appointments', 'Booking failed: invalid externalId: @book_data', array('@book_data' => var_export($book_data, 1)), WATCHDOG_ERROR);
      throw new Exception('Booking failed. No email found.', DVG_APPOINTMENTS_ERRORCODE_UNKNOWN);
    }
    $base_info = array(
      'firstName' => (isset($book_data['firstName'])) ? $book_data['firstName'] : '',
      'lastName' => (isset($book_data['lastName'])) ? $book_data['lastName'] : '',
      'phone' => (isset($book_data['phone'])) ? $book_data['phone'] : '',
      'email' => (isset($book_data['email'])) ? strtolower($book_data['email']) : '',
      'dateOfBirth' => (isset($book_data['dateOfBirth'])) ? $book_data['dateOfBirth'] : '',
      'externalId' => (isset($book_data['externalId'])) ? $book_data['externalId'] : '',
    );

    $appointment = new DvgAppointment();
    $appointment->setStart(DateTime::createFromFormat('U', $appointment_data['datetime']));
    $start_time = $appointment_data['datetime'];
    foreach ($appointment_data['products'] as $product) {
      for ($i = 1; $i <= $product['count']; $i++) {
        $duration = $product['duration'] * 60;
        $end_date = dvg_appointments_format_date($start_time + $duration, 'custom', 'c');

        $date = dvg_appointments_format_date($start_time, 'custom', 'Y-m-d');
        $time = dvg_appointments_format_date($start_time, 'custom', 'H:i');
        $book_appointment = $this->call('bookGovAppointment', array(
          'rest_url' => '/branches/' . $this->branch_id . '/services/' . $product['id'] . '/dates/' . $date . '/times/' . $time . '/book',
          'rest_method' => 'POST',
          'title' => 'Online booking',
          'notes' => '',
          'customer' => $base_info,
        ), TRUE);

        if (isset($book_appointment->code) && isset($book_appointment->error)) {
          watchdog('dvg_appointments', 'Booking failed: "@error" (@code). @message', array(
            '@code' => $book_appointment->code,
            '@error' => $book_appointment->error,
            '@message' => $book_appointment->msg,
          ), WATCHDOG_ERROR);
          foreach ($appointment->getRemoteAppointmentIds() as $remote_id) {
            $this->delete_appointment($remote_id);
          }

          throw new Exception('Booking failed. No appointment id.', $book_appointment->code);
        }
        else {
          $appointment->addProduct($product['id'], 1, $duration);
          $appointment->addRemoteAppointmentId($book_appointment->publicId);
        }

        $start_time += $duration;
      }
    }

    if (isset($end_date)) {
      $appointment->setEnd(DateTime::createFromFormat('U', $start_time));
    }

    return $appointment;
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
      'firstName' => 'First Name',
      'lastName' => 'Last Name',
      'phone' => 'Telephone',
      'email' => 'Email',
      'dateOfBirth' => 'Birthdate',
    );
  }

  protected function get_cache_expire() {
    return array(
      'getAvailableAppointmentTimes' => 10,
      'getAvailableProducts' => 60 * 60 * 24,
    );
  }
}
