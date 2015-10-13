<?php

/**
 * @file
 * The default format for Viet Nam adresses.
 */
// Plugin name.
$plugin = array(
  'title' => t('Address form (specific for Viet Nam)'),
  'format callback' => 'addressfield_vn_format_address_generate',
  'type' => 'address',
  'weight' => -100,
);

/**
 * Format callback for Viet Nam address.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_vn_format_address_generate(&$format, $address, $context = array()) {
  if ($address['country'] == 'VN') {
    unset($format['locality_block']['administrative_area']);

    $format['locality_block']['locality'] = array(
      '#title' => t('City/Province'),
      '#options' => _addressfield_vn_city_list(),
      '#required' => TRUE,
      '#attributes' => array('class' => array('locality')),
      '#weight' => 1,
    );
    $format['locality_block']['dependent_locality'] = array(
      '#title' => t('District'),
      '#size' => 20,
      '#tag' => 'div',
      '#attributes' => array('class' => array('dependent', 'district')),
      '#weight' => 2,
    );
    $format['locality_block']['postal_code'] = array(
      '#title' => t('Postal Code'),
      '#size' => 10,
      '#required' => TRUE,
      '#attributes' => array('class' => array('postal-code')),
      '#weight' => 3,
    );
    // Default district.
    if ($address['locality'] == 'Ho Chi Minh') {
      $dist = array(
        t('Quan 1'),
        t('Quan 2'),
        t('Quan 3'),
        t('Quan 4'),
        t('Quan 5'),
        t('Quan 6'),
        t('Quan 7'),
        t('Quan 8'),
        t('Quan 9'),
        t('Quan 10'),
        t('Quan 11'),
        t('Quan 12'),
        t('Go Vap'),
        t('Tan Binh'),
        t('Tan Phu'),
        t('Binh Thanh'),
        t('Phu Nhuan'),
        t('Thu Duc'),
        t('Binh Tan'),
        t('Cu Chi'),
        t('Hoc Mon'),
        t('Binh Chanh'),
        t('Nha Be'),
        t('Can Gio'),
      );
      $dists = array_combine($dist, $dist);
      $format['locality_block']['dependent_locality']['#options'] = $dists;
    }
    elseif ($address['locality'] == 'Ha Noi') {
      $dist = array(
        t('Ba Dinh'),
        t('Cau Giay'),
        t('Dong Da'),
        t('Hai Ba Trung'),
        t('Hoan Kiem'),
        t('Hoang Mai'),
        t('Long Bien'),
        t('Tay Ho'),
        t('Thanh Xuan'),
        t('Ha Dong'),
        t('Huyen Dong Anh'),
        t('Huyen Gia Lam'),
        t('Huyen Soc Son'),
        t('Huyen Thanh Tri'),
        t('Huyen Tu Liem'),
        t('Thi Xa Son Tay'),
        t('Huyen Ba Vi'),
        t('Huyen Chuong My'),
        t('Huyen Dan Phuong'),
        t('Huyen Hoai Duc'),
        t('Huyen My Duc'),
        t('Huyen Phu Xuyen'),
        t('Huyen Phuc Tho'),
        t('Huyen Quoc Oai'),
        t('Huyen Thach That'),
        t('Huyen Thanh Oai'),
        t('Huyen Thuong Tin'),
        t('Huyen Ung Hoa'),
        t('Huyen Me Linh'),
      );
      $dists = array_combine($dist, $dist);
      $format['locality_block']['dependent_locality']['#options'] = $dists;
    }
    elseif ($address['locality'] == 'Da Nang') {
      $dist = array(
        t('Hai Chau'),
        t('Thanh Khe'),
        t('Son Tra'),
        t('Ngu Hanh Son'),
        t('Lien Chieu'),
        t('Cam Le'),
        t('Huyen Hoa Vang'),
        t('Huyen Dao Hoang Sa'),
      );
      $dists = array_combine($dist, $dist);
      $format['locality_block']['dependent_locality']['#options'] = $dists;
    }
    elseif ($address['locality'] == 'Hai Phong') {
      $dist = array(
        t('Duong Kinh'),
        t('Do Son'),
        t('Hai An'),
        t('Kien An'),
        t('Hong Bang'),
        t('Ngo Quyen'),
        t('Le Chan'),
        t('Huyen An Duong'),
        t('Huyen An Lao'),
        t('Huyen Dao Bach Long Vi'),
        t('Huyen Cat Hai'),
        t('Huyen Kien Thuy'),
        t('Huyen Tien Lang'),
        t('Huyen Vinh Bao'),
        t('Huyen Thuy Nguyen'),
      );
      $dists = array_combine($dist, $dist);
      $format['locality_block']['dependent_locality']['#options'] = $dists;
    }
    elseif ($address['locality'] == 'Can Tho') {
      $dist = array(
        t('Ninh Kieu'),
        t('Binh Thuy'),
        t('Cai Rang'),
        t('O Mon'),
        t('Thot Not'),
        t('Huyen Phong Dien'),
        t('Huyen Co Do'),
        t('Huyen Thoi Lai'),
        t('Huyen Vinh Thanh'),
      );
      $dists = array_combine($dist, $dist);
      $format['locality_block']['dependent_locality']['#options'] = $dists;
    }
  }
  // Format render.
  if ($context['mode'] == 'render' && $address['country'] == 'VN') {
    $format['locality_block']['dependent_locality']['#weight'] = 1;
    $format['locality_block']['locality']['#weight'] = 2;
    $format['locality_block']['postal_code']['#weight'] = 3;
    $format['locality_block']['postal_code']['#prefix'] = ' ';
  }
  // Check ajax form in Viet Nam.
  if ($context['mode'] == 'form' && $address['country'] == 'VN') {
    $format['locality_block']['locality']['#ajax'] = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
      'method' => 'replace',
    );
  }
  else {
    if (isset($format['locality_block']['locality'])) {
      $format['locality_block']['locality']['#ajax'] = array();
    }
  }
}

/**
 * The list city/province in Viet Nam.
 */
function _addressfield_vn_city_list() {
  // Using http://goo.gl/oDHJlc.
  $data = array(
    '' => t('--'),
    t('Ha Noi') => t('Ha Noi'),
    t('Ho Chi Minh') => t('Ho Chi Minh'),
    t('Da Nang') => t('Da Nang'),
    t('Hai Phong') => t('Hai Phong'),
    t('Can Tho') => t('Can Tho'),
    t('An Giang') => t('An Giang'),
    t('Ba Ria - Vung Tau') => t('Ba Ria - Vung Tau'),
    t('Bac Lieu') => t('Bac Lieu'),
    t('Bac Giang') => t('Bac Giang'),
    t('Bac Can') => t('Bac Can'),
    t('Bac Ninh') => t('Bac Ninh'),
    t('Ben Tre') => t('Ben Tre'),
    t('Binh Duong') => t('Binh Duong'),
    t('Binh Phuoc') => t('Binh Phuoc'),
    t('Binh Thuan') => t('Binh Thuan'),
    t('Ca Mau') => t('Ca Mau'),
    t('Cao Bang') => t('Cao Bang'),
    t('Dac Lac') => t('Dac Lac'),
    t('Dac Nong') => t('Dac Nong'),
    t('Dien Bien') => t('Dien Bien'),
    t('Dong Nai') => t('Dong Nai'),
    t('Dong Thap') => t('Dong Thap'),
    t('Gia Lai') => t('Gia Lai'),
    t('Ha Giang') => t('Ha Giang'),
    t('Ha Nam') => t('Ha Nam'),
    t('Ha Tinh') => t('Ha Tinh'),
    t('Hai Duong') => t('Hai Duong'),
    t('Hau Giang') => t('Hau Giang'),
    t('Hoa Binh') => t('Hoa Binh'),
    t('Hung Yen') => t('Hung Yen'),
    t('Khanh Hoa') => t('Khanh Hoa'),
    t('Kien Giang') => t('Kien Giang'),
    t('Kon Tum') => t('Kon Tum'),
    t('Lai Chau') => t('Lai Chau'),
    t('Lam Dong') => t('Lam Dong'),
    t('Lang Son') => t('Lang Son'),
    t('Lao Cai') => t('Lao Cai'),
    t('Long An') => t('Long An'),
    t('Nam Dinh') => t('Nam Dinh'),
    t('Nghe An') => t('Nghe An'),
    t('ninh Binh') => t('ninh Binh'),
    t('Ninh Thuan') => t('Ninh Thuan'),
    t('Phu Tho') => t('Phu Tho'),
    t('Phu Yen') => t('Phu Yen'),
    t('Quang Binh') => t('Quang Binh'),
    t('Quang Nam') => t('Quang Nam'),
    t('Quang Ngai') => t('Quang Ngai'),
    t('Quang Ninh') => t('Quang Ninh'),
    t('Quang Tri') => t('Quang Tri'),
    t('Soc Trang') => t('Soc Trang'),
    t('Son La') => t('Son La'),
    t('Tay Ninh') => t('Tay Ninh'),
    t('Thai Binh') => t('Thai Binh'),
    t('Thai Nguyen') => t('Thai Nguyen'),
    t('Thanh Hoa') => t('Thanh Hoa'),
    t('Thua Thien - Hue') => t('Thua Thien - Hue'),
    t('Tien Giang') => t('Tien Giang'),
    t('Tra Vinh') => t('Tra Vinh'),
    t('Tuyen Quang') => t('Tuyen Quang'),
    t('Vinh Long') => t('Vinh Long'),
    t('Vinh Phuc') => t('Vinh Phuc'),
    t('Yen Bai') => t('Yen Bai'),
  );

  return $data;
}
