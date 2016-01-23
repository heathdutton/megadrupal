<?php

/**
 * @file
 * Php class file for the phplot api project.
 *
 * This provides the phplot_api_plot class object.
 */

/**
 * Plot class object for creating phplots.
 */
class phplot_api_plot {
  protected $type = NULL;
  protected $data = array();
  protected $settings = array();
  protected $image = NULL;
  protected $file = NULL;
  protected $plot = NULL;


  /**
   * Class Initailizer.
   *
   * @param string type
   *   A machine name string of the plot type.
   */
  public function phplot_api_plot($type) {
    $this->set_type($type);

    foreach ($this->settings['defaults'] as $feature => $value) {
      $plot_data[$feature] = $value;
    }
  }

  /**
   * Returns the type of this plot.
   *
   * @return string
   *   A machine name string of the plot type.
   */
  public function get_type() {
    return $this->type;
  }

  /**
   * Returns the data assigned to this plot.
   *
   * @param string|null feature
   *   (optional) A machine name string of a specific setting to get.
   *   If NULL, then the entire plot data array is returned.
   *
   * @return
   *   Either the entire plot data array or a specific value for a specified feature.
   */
  public function get_data(string $feature = NULL) {
    if (is_null($feature)) {
      return $this->data;
    }

    // @todo throw an error if 'feature' does not exist in the array.
    return $this->data[$feature];
  }

  /**
   * Returns the image of this plot.
   *
   * @return
   *   A binary image blob.
   */
  public function get_image() {
    return $this->image;
  }

  /**
   * Returns the image file of this plot.
   *
   * @return object
   *   A file object that represents the plot image.
   */
  public function get_file() {
    return $this->file;
  }

  /**
   * Returns the size of the legend.
   *
   * @return object
   *   A file object that represents the plot image.
   */
  public function get_legend_size() {
    if (is_object($this->plot)) {
      return $this->plot->GetLegendSize();
    }

    return array(0, 0);
  }

  /**
   * Returns a translation of world coordinates into device coordinates.
   *
   * @param x_world
   *   The X coordinate to translate from world coordinates.
   * @param y_world
   *   The Y coordinate to translate from world coordinates.
   *
   * @return object
   *   A file object that represents the plot image.
   */
  public function get_device_xy($x_world, $y_world) {
    if (is_object($this->plot)) {
      return $this->plot->GetDeviceXY($x_world, $y_world);
    }

    return array(0, 0);
  }

  /**
   * Sets the type of this plot.
   *
   * @param string type
   *   A machine name string of the plot type.
   */
  public function set_type($type) {
    $this->settings = phplot_api_get_plots($type);
    $this->type = $type;
  }

  /**
   * Assign some value to a given feature.
   *
   * @param string feature
   *   The machine name of the feature.
   * @param value
   *   Data to assign to the specified feature.
   * @param bool reset
   *   (optional) Whether or not the data should be cleared before assigning a value.
   *   This only does something if the feature supports
   *   PHPLOT_API_FEATURE_MULTIPLE.
   *
   * @return bool
   *   TRUE is returned on success and FALSE on any failure.
   */
  public function set_data($feature, $value, $reset = FALSE) {
    if (!cf_has_array_key($feature, $this->settings['features'])) {
      if (class_exists('cf_error')) {
        cf_error::missing_array_key('this->settings', 'features', WATCHDOG_WARNING);
      }

      return FALSE;
    }

    // @todo perform data integrity checks here before assigning the value
    if ($this->settings['features'][$feature] == PHPLOT_API_FEATURE_NONE || $this->settings['features'][$feature] == PHPLOT_API_FEATURE_SINGLE) {
      $this->data[$feature] = $value;
    }
    else if ($this->settings['features'][$feature] == PHPLOT_API_FEATURE_MULTIPLE) {
      if ($reset || !isset($this->data[$feature])) {
        $this->data[$feature] = array();
      }

      array_push($this->data[$feature], $value);
    }

    return TRUE;
  }

  /**
   * Builds the plot image file.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  public function build() {
    $library = libraries_load('phplot');

    if (empty($library['loaded'])) {
      if (class_exists('cf_error')) {
        cf_error::failed_to_load('quail-lib');
      }

      return FALSE;
    }

    if (isset($this->data['width'])) {
      $width = $this->data['width'];

      if (!is_numeric($width) || $width <= 0) {
        $width = NULL;
      }
    }
    else {
      $width = NULL;
    }

    if (isset($this->data['height'])) {
      $height = $this->data['height'];

      if (!is_numeric($height) || $height <= 0) {
        $height = NULL;
      }
    }
    else {
      $height = NULL;
    }

    // Create a PHPlot object with the given dimensions
    if (is_null($width) || is_null($height)) {
      $this->plot = new PHPlot();
    }
    else {
      $this->plot = new PHPlot($width, $height);
    }

    if (isset($this->data['data_type'])) {
      $this->plot->SetDataType($this->data['data_type']);
    }

    if (isset($this->data['data_colors'])) {
      $item = $this->data['data_colors'];

      switch(count($item)) {
        case 3:
          $this->plot->SetDataColors($item[0], $item[1], $item[2]);
          break;
        case 2:
          $this->plot->SetDataColors($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetDataColors($item[0]);
          break;
      }
    }

    if (isset($this->data['data_border_colors'])) {
      $this->plot->SetDataBorderColors($this->data['data_border_colors']);
    }

    if (isset($this->data['data_values'])) {
      $this->plot->SetDataValues($this->data['data_values']);
    }

    if (isset($this->data['data_label_angle_x'])) {
      $this->plot->SetXDataLabelAngle($this->data['data_label_angle_x']);
    }

    if (isset($this->data['data_label_position_x'])) {
      $this->plot->SetXDataLabelPos($this->data['data_label_position_x']);
    }

    if (isset($this->data['data_label_type_x'])) {
      $this->plot->SetXDataLabelType($this->data['data_label_type_x']);
    }

    if (isset($this->data['data_label_angle_y'])) {
      $this->plot->SetXDataLabelAngle($this->data['data_label_angle_y']);
    }

    if (isset($this->data['data_label_position_y'])) {
      $this->plot->SetXDataLabelPos($this->data['data_label_position_y']);
    }

    if (isset($this->data['data_label_type_y'])) {
      $this->plot->SetXDataLabelType($this->data['data_label_type_y']);
    }

    if (isset($this->data['title'])) {
      $this->plot->SetTitle($this->data['title']);
    }

    if (isset($this->data['title_color'])) {
      $this->plot->SetTitleColor($this->data['title_color']);
    }

    if (isset($this->data['title_font_size'])) {
      $this->plot->SetTitleFontSize($this->data['title_font_size']);
    }

    if (isset($this->data['ttf_path'])) {
      $this->plot->SetTTFPath($this->data['ttf_path']);
    }

    if (isset($this->data['ttf_default'])) {
      $this->plot->SetDefaultTTFont($this->data['ttf_default']);
    }

    if (isset($this->data['ttf_font'])) {
      foreach ($this->data['ttf_font'] as $item) {
        switch(count($item)) {
          case 4:
            $this->plot->SetFontTTF($item[0], $item[1], $item[2], $item[3]);
            break;
          case 3:
            $this->plot->SetFontTTF($item[0], $item[1], $item[2]);
            break;
          case 2:
            $this->plot->SetFontTTF($item[0], $item[1]);
            break;
        }
      }
    }

    if (isset($this->data['background_color'])) {
      $this->plot->SetBackgroundColor($this->data['background_color']);
    }

    if (isset($this->data['margins_pixels'])) {
      $item = $this->data['margins_pixels'];

      switch(count($item)) {
        case 4:
          $this->plot->SetMarginsPixels($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $this->plot->SetMarginsPixels($item[0], $item[1], $item[2]);
          break;
        case 2:
          $this->plot->SetMarginsPixels($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetMarginsPixels($item[0]);
          break;
      }
    }

    if (isset($this->data['plot_area_background_draw'])) {
      $this->plot->SetDrawPlotAreaBackground($this->data['plot_area_background_draw']);
    }

    if (isset($this->data['plot_background_color'])) {
      $this->plot->SetPlotBgColor($this->data['plot_background_color']);
    }

    if (isset($this->data['precision_x'])) {
      $this->plot->SetPrecisionX($this->data['precision_x']);
    }

    if (isset($this->data['precision_y'])) {
      $this->plot->SetPrecisionY($this->data['precision_y']);
    }

    if (isset($this->data['shading'])) {
      $this->plot->SetShading($this->data['shading']);
    }

    if (isset($this->data['label_color'])) {
      $this->plot->SetLabelColor($this->data['label_color']);
    }

    if (isset($this->data['label_type'])) {
      foreach ($this->data['label_type'] as $item) {
        if (count($item) == 2) {
          $this->plot->SetLabelType($item[0], $item[1]);
        }
      }
    }

    if (isset($this->data['label_scale_position'])) {
      $this->plot->SetLabelScalePosition($this->data['label_scale_position']);
    }

    // @todo is this a special case where if plot_area_world is an empty array it should call: $this->plot->SetPlotAreaWorld(); ?
    if (isset($this->data['plot_area_world'])) {
      $item = $this->data['plot_area_world'];

      switch(count($item)) {
        case 4:
          $this->plot->SetPlotAreaWorld($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $this->plot->SetPlotAreaWorld($item[0], $item[1], $item[2]);
          break;
        case 2:
          $this->plot->SetPlotAreaWorld($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetPlotAreaWorld($item[0]);
          break;
      }
    }

    if (isset($this->data['plot_area_background_image'])) {
      $plot_area_background_image = $this->data['plot_area_background_image'];

      if (count($plot_area_background_image) == 2) {
        $this->plot->SetLabelType($plot_area_background_image[0], $plot_area_background_image[1]);
      }
    }

    // @todo is this a special case where if plot_area_pixels is an empty array it should call: $this->plot->SetPlotAreaPixels(); ?
    if (isset($this->data['plot_area_pixels'])) {
      $item = $this->data['plot_area_pixels'];

      switch(count($item)) {
        case 4:
          $this->plot->SetPlotAreaPixels($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $this->plot->SetPlotAreaPixels($item[0], $item[1], $item[2]);
          break;
        case 2:
          $this->plot->SetPlotAreaPixels($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetPlotAreaPixels($item[0]);
          break;
      }
    }

    if (isset($this->data['image_border_color'])) {
      $this->plot->SetImageBorderColor($this->data['image_border_color']);
    }

    if (isset($this->data['image_border_type'])) {
      $this->plot->SetImageBorderType($this->data['image_border_type']);
    }

    if (isset($this->data['image_border_width'])) {
      $this->plot->SetImageBorderWidth($this->data['image_border_width']);
    }

    if (isset($this->data['tick_color'])) {
      $this->plot->SetTickColor($this->data['tick_color']);
    }

    if (isset($this->data['tick_length'])) {
      $this->plot->SetTickLength($this->data['tick_length']);
    }

    if (isset($this->data['legend'])) {
      foreach ($this->data['legend'] as $item) {
        $this->plot->SetLegend($item);
      }
    }

    if (isset($this->data['legend_pixels'])) {
      $item = $this->data['legend_pixels'];

      switch(count($item)) {
        case 2:
          $this->plot->SetDataColors($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetDataColors($item[0]);
          break;
      }
    }

    if (isset($this->data['legend_position'])) {
      $item = $this->data['legend_position'];

      switch(count($item)) {
        case 7:
          $this->plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3], $item[4], $item[5], $item[6]);
          break;
        case 6:
          $this->plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3], $item[4], $item[5]);
          break;
        case 5:
          $this->plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3], $item[4]);
          break;
        case 4:
          $this->plot->SetLegendPosition($item[0], $item[1], $item[2], $item[3]);
          break;
        case 3:
          $this->plot->SetLegendPosition($item[0], $item[1], $item[2]);
          break;
        case 2:
          $this->plot->SetLegendPosition($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetLegendPosition($item[0]);
          break;
      }
    }

    if (isset($this->data['legend_reverse'])) {
      $this->plot->SetLegendReverse($this->data['legend_reverse']);
    }

    if (isset($this->data['legend_style'])) {
      $item = $this->data['legend_style'];

      switch(count($item)) {
        case 2:
          $this->plot->SetLegendStyle($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetLegendStyle($item[0]);
          break;
      }
    }

    if (isset($this->data['legend_use_shapes'])) {
      $this->plot->SetLegendUseShapes($this->data['legend_use_shapes']);
    }

    if (isset($this->data['legend_world'])) {
      $item = $this->data['legend_world'];

      switch(count($item)) {
        case 2:
          $this->plot->SetLegendWorld($item[0], $item[1]);
          break;
        case 1:
          $this->plot->SetLegendWorld($item[0]);
          break;
      }
    }

    if (isset($this->data['tick_anchor_x'])) {
      $this->plot->SetXTickAnchor($this->data['tick_anchor_x']);
    }

    if (isset($this->data['tick_crossing_x'])) {
      $this->plot->SetXTickCrossing($this->data['tick_crossing_x']);
    }

    if (isset($this->data['tick_increment_x'])) {
      $this->plot->SetXTickIncrement($this->data['tick_increment_x']);
    }

    if (isset($this->data['tick_label_position_x'])) {
      $this->plot->SetXTickLabelPos($this->data['tick_label_position_x']);
    }

    if (isset($this->data['tick_length_x'])) {
      $this->plot->SetXTickLength($this->data['tick_length_x']);
    }

    if (isset($this->data['tick_position_x'])) {
      $this->plot->SetXTickPos($this->data['tick_position_x']);
    }

    if (isset($this->data['tick_anchor_y'])) {
      $this->plot->SetXTickAnchor($this->data['tick_anchor_y']);
    }

    if (isset($this->data['tick_crossing_y'])) {
      $this->plot->SetXTickCrossing($this->data['tick_crossing_y']);
    }

    if (isset($this->data['tick_increment_y'])) {
      $this->plot->SetXTickIncrement($this->data['tick_increment_y']);
    }

    if (isset($this->data['tick_label_position_y'])) {
      $this->plot->SetXTickLabelPos($this->data['tick_label_position_y']);
    }

    if (isset($this->data['tick_length_y'])) {
      $this->plot->SetXTickLength($this->data['tick_length_y']);
    }

    if (isset($this->data['tick_position_y'])) {
      $this->plot->SetXTickPos($this->data['tick_position_y']);
    }

    // @todo provide a modules_invoke call so that modules that provide their own tweaks can alter the plot object.

    return TRUE;
  }

  /**
   * Generates and image blob file.
   *
   * @return
   *   TRUE if no problems were detected and FALSE if a problem was detected.
   *
   * @see file_default_scheme()
   */
  function create_image() {
    // Enforce the defined type here to prevent extending modules from improperly altering this.
    $this->plot->SetPlotType($this->type);

    // The image must be inline for it to be loaded and returned in this way.
    $this->plot->SetIsInline(TRUE);

    // Draw the graph into a buffer $image
    ob_start();
    $this->plot->DrawGraph();
    $this->image = ob_get_contents();
    ob_end_clean();
  }

  /**
   * Saves the phplot image block to the filesystem and the drupal database.
   *
   * The plot file is saved as a temporary file.
   *
   * @param int file_status
   *   (optional) Define whether or not a file is permanent or temporary.
   *   Temporary is represented by 0 (for some reason drupal core does not
   *   define a global constant for this).
   *   Permanent is represented by FILE_STATUS_PERMANENT.
   *   Temporary is the default behavior.
   *
   * @param directory
   *   (optional) A string defining the directory structure where the file will
   *   be saved.
   *   This should not begin with a '/'.
   * @param string|null file_schema
   *   (optional) The file schema to use.
   *   - if NULL, then file_default_scheme() is used.
   *
   * @return object|false
   *   The file object that was saved to the database or FALSE on error.
   *
   * @see phplot_api_build_plot()
   * @see file_default_scheme()
   */
  function create_file($file_status = 0, $directory = 'statistics/plots', $file_schema = NULL) {
    // @todo how does one validate the image blob?
    if (empty($this->image)) {
      return FALSE;
    }

    if (is_null($file_schema)) {
      $file_schema = phplot_api_default_scheme();
    }

    global $user;
    $filename = 'plot-' . sha1($this->image) . '.png';
    $directory =  $file_schema . '://' . $directory;
    $destination = $directory . '/' . $filename;
    $filepath = '';

    file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);

    if ($file_status == 0) {
      $new_file = !file_exists($destination);
      $filepath = file_unmanaged_save_data($this->image, $destination, FILE_EXISTS_REPLACE);

      if ($new_file) {
        $this->file = new stdClass();
        $this->file->fid = NULL;
        $this->file->uri = $filepath;
        $this->file->filename = basename($filename);
        $this->file->filemime = 'image/png';
        $this->file->filesize = filesize($filepath);
        $this->file->uid = $user->uid;
        $this->file->status = 0;

        file_save($this->file);
      }
      else {
        // @todo what should be done here is to attempt to load the file object from the database and return that file object
      }
    }
    else if ($file_status == FILE_STATUS_PERMANENT) {
      $this->file = file_save_data($this->image, $destination, FILE_EXISTS_REPLACE);

      $filepath = $this->file->uri;
    }

    return $filepath;
  }
}
