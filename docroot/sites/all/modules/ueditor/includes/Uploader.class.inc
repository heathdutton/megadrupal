<?php

/**
 * Created by JetBrains PhpStorm.
 * User: taoqili
 * Date: 12-7-18
 * Time: 上午11: 32
 * UEditor编辑器通用上传类
 */
class Uploader {
    private $fileField; //文件域名
    private $file; //文件上传对象
    private $base64; //文件上传对象
    private $config; //配置信息
    private $oriName; //原始文件名
    private $fileName; //新文件名
    private $fullName; //完整文件名,即从当前配置目录开始的URL
    private $filePath; //完整文件名,即从当前配置目录开始的URL
    private $fileSize; //文件大小
    private $fileType; //文件类型
    private $stateInfo; //上传状态信息,
    private $stateMap = array( //上传状态映射表，国际化用户需考虑此处数据的国际化
        "SUCCESS", //上传成功标记，在UEditor中内不可改变，否则flash判断会出错
        "文件大小超出 upload_max_filesize 限制",
        "文件大小超出 MAX_FILE_SIZE 限制",
        "文件未被完整上传",
        "没有文件被上传",
        "上传文件为空",
        "ERROR_TMP_FILE" => "临时文件错误",
        "ERROR_TMP_FILE_NOT_FOUND" => "找不到临时文件",
        "ERROR_SIZE_EXCEED" => "文件大小超出网站限制",
        "ERROR_TYPE_NOT_ALLOWED" => "文件类型不允许",
        "ERROR_CREATE_DIR" => "目录创建失败",
        "ERROR_DIR_NOT_WRITEABLE" => "目录没有写权限",
        "ERROR_FILE_MOVE" => "文件保存时出错",
        "ERROR_FILE_NOT_FOUND" => "找不到上传文件",
        "ERROR_WRITE_CONTENT" => "写入文件内容错误",
        "ERROR_UNKNOWN" => "未知错误",
        "ERROR_WATERMARK_ADD" => "添加水印出错",
        "ERROR_WATERMARK_TEXT_RGB" => "水印文字颜色格式不正确",
        "ERROR_WATERMARK_NOT_FOUND" => "需要添加水印的图片不存在",
        "ERROR_WATERMARK_SIZE" => "水印太大或图片太小",
        "ERROR_DEAD_LINK" => "链接不可用",
        "ERROR_HTTP_LINK" => "链接不是http链接",
        "ERROR_HTTP_CONTENTTYPE" => "链接contentType不正确"
    );

    /**
     * 构造函数
     * @param string $fileField 表单名称
     * @param array $config 配置项
     * @param bool $base64 是否解析base64编码，可省略。若开启，则$fileField代表的是base64编码的字符串表单名
     */
    public function __construct($fileField, $config, $type = "upload") {
        $this->fileField = $fileField;
        $this->config = $config;
        $this->type = $type;
        if ($type == "remote") {
            $this->saveRemote();
        } else if($type == "base64") {
            $this->upBase64();
        } else {
            $this->upFile();
        }

        $this->stateMap['ERROR_TYPE_NOT_ALLOWED'] = iconv('unicode', 'utf-8', $this->stateMap['ERROR_TYPE_NOT_ALLOWED']);
    }

    /**
     * 上传文件的主处理方法
     * @return mixed
     */
    private function upFile() {        
        $file = $this->file = $_FILES[$this->fileField];
        if (!$file) {
            $this->stateInfo = $this->getStateInfo("ERROR_FILE_NOT_FOUND");
            return;
        }
        if ($this->file['error']) {
            $this->stateInfo = $this->getStateInfo($file['error']);
            return;
        } else if (!file_exists($file['tmp_name'])) {
            $this->stateInfo = $this->getStateInfo("ERROR_TMP_FILE_NOT_FOUND");
            return;
        } else if (!is_uploaded_file($file['tmp_name'])) {
            $this->stateInfo = $this->getStateInfo("ERROR_TMPFILE");
            return;
        }

        $this->oriName = $file['name'];
        $this->fileSize = $file['size'];
        $this->fileType = $this->getFileExt();
        $this->fullName = $this->getFullName();
        $this->savePath = ueditor_get_savepath($this->fullName);
        $this->filePath = $this->getFilePath();
        $this->fileName = $this->getFileName();
        $dirname = dirname($this->filePath);

        //检查文件大小是否超出限制
        if (!$this->checkSize()) {
            $this->stateInfo = $this->getStateInfo("ERROR_SIZE_EXCEED");
            return;
        }

        //检查是否不允许的文件格式
        if (!$this->checkType()) {
            $this->stateInfo = $this->getStateInfo("ERROR_TYPE_NOT_ALLOWED");
            return;
        }

        //创建目录失败
        if (!file_exists($dirname) && !mkdir($dirname, 0777, true)) {
            $this->stateInfo = $this->getStateInfo("ERROR_CREATE_DIR");
            return;
        } else if (!is_writeable($dirname)) {
            $this->stateInfo = $this->getStateInfo("ERROR_DIR_NOT_WRITEABLE");
            return;
        }
        // Load the files contents
        $imagedata = file_get_contents($file["tmp_name"]);
        
        $filedata = $this->savefileToDrupal($imagedata, $this->savePath);

        //移动文件
        if (is_object($filedata)) { //移动成功
          $success = FALSE;
          $use_watermark = variable_get('ueditor_watermark',0);
          if($use_watermark){
            $success = $this->addWatermark($this->filePath);
          }
          if($success === TRUE){
            $this->stateInfo = $this->stateMap[0];
          }else{
            $this->stateInfo = $this->getStateInfo($success);
          }
        } else { //移动失败
          $this->stateInfo = $this->getStateInfo("ERROR_FILE_MOVE");
        }
    }

    /**
     * 处理base64编码的图片上传
     * @return mixed
     */
    private function upBase64() {
        $base64Data = $_POST[$this->fileField];
        $img = base64_decode($base64Data);

        $this->oriName = $this->config['oriName'];
        $this->fileSize = strlen($img);
        $this->fileType = $this->getFileExt();
        $this->fullName = $this->getFullName();
        $this->savePath = ueditor_get_savepath($this->fullName);
        $this->filePath = $this->getFilePath();
        $this->fileName = $this->getFileName();
        $dirname = dirname($this->filePath);

        //检查文件大小是否超出限制
        if (!$this->checkSize()) {
            $this->stateInfo = $this->getStateInfo("ERROR_SIZE_EXCEED");
            return;
        }

        //创建目录失败
        if (!file_exists($dirname) && !mkdir($dirname, 0777, true)) {
            $this->stateInfo = $this->getStateInfo("ERROR_CREATE_DIR");
            return;
        } else if (!is_writeable($dirname)) {
            $this->stateInfo = $this->getStateInfo("ERROR_DIR_NOT_WRITEABLE");
            return;
        }

        $filedata = $this->savefileToDrupal($img, $this->savePath);

        //移动文件
        if (is_object($filedata)) { //移动成功
          $success = FALSE;
          $use_watermark = variable_get('ueditor_watermark',0);
          if($use_watermark){
            $success = $this->addWatermark($this->filePath);
          }
          if($success === TRUE){
            $this->stateInfo = $this->stateMap[0];
          }else{
            $this->stateInfo = $this->getStateInfo($success);
          }
        } else { //移动失败
          $this->stateInfo = $this->getStateInfo("ERROR_WRITE_CONTENT");
        }

    }

    /**
     * 拉取远程图片
     * @return mixed
     */
    private function saveRemote() {
        $imgUrl = htmlspecialchars($this->fileField);
        $imgUrl = str_replace("&amp;", "&", $imgUrl);

        //http开头验证
        if (strpos($imgUrl, "http") !== 0) {
            $this->stateInfo = $this->getStateInfo("ERROR_HTTP_LINK");
            return;
        }
        //获取请求头并检测死链
        $heads = get_headers($imgUrl);
        if (!(stristr($heads[0], "200") && stristr($heads[0], "OK"))) {
            $this->stateInfo = $this->getStateInfo("ERROR_DEAD_LINK");
            return;
        }
        //格式验证(扩展名验证和Content-Type验证)
        $fileType = strtolower(strrchr($imgUrl, '.'));
        if (!in_array($fileType, $this->config['allowFiles']) || stristr($heads['Content-Type'], "image")) {
            $this->stateInfo = $this->getStateInfo("ERROR_HTTP_CONTENTTYPE");
            return;
        }

        //打开输出缓冲区并获取远程图片
        ob_start();
        $context = stream_context_create(
            array('http' => array(
                'follow_location' => false // don't follow redirects
            ))
        );
        readfile($imgUrl, false, $context);
        $img = ob_get_contents();
        ob_end_clean();
        preg_match("/[\/]([^\/]*)[\.]?[^\.\/]*$/", $imgUrl, $m);

        $this->oriName = $m ? $m[1]:"";
        $this->fileSize = strlen($img);
        $this->fileType = $this->getFileExt();
        $this->fullName = $this->getFullName();
        $this->savePath = ueditor_get_savepath($this->fullName);
        $this->filePath = $this->getFilePath();
        $this->fileName = $this->getFileName();
        $dirname = dirname($this->filePath);

        //检查文件大小是否超出限制
        if (!$this->checkSize()) {
            $this->stateInfo = $this->getStateInfo("ERROR_SIZE_EXCEED");
            return;
        }

        //创建目录失败
        if (!file_exists($dirname) && !mkdir($dirname, 0777, true)) {
            $this->stateInfo = $this->getStateInfo("ERROR_CREATE_DIR");
            return;
        } else if (!is_writeable($dirname)) {
            $this->stateInfo = $this->getStateInfo("ERROR_DIR_NOT_WRITEABLE");
            return;
        }

        $filedata = $this->savefileToDrupal($img, $this->savePath);

        //移动文件
        if (is_object($filedata)) { //移动成功
          $success = FALSE;
          $use_watermark = variable_get('ueditor_watermark',0);
          if($use_watermark){
            $success = $this->addWatermark($this->filePath);
          }
          if($success === TRUE){
            $this->stateInfo = $this->stateMap[0];
          }else{
            $this->stateInfo = $this->getStateInfo($success);
          }
        } else { //移动失败
          $this->stateInfo = $this->getStateInfo("ERROR_WRITE_CONTENT");
        }
    }

    /**
     * 上传保存文件到drupal
     * @param $imagedata
     * @param $savePath
     * @return string
     */
    private function savefileToDrupal($imagedata, $savePath) {
        global $user;
        $filedata = '';
        //save the file to file_managed table.
        if ($uri = file_unmanaged_save_data($imagedata, 'public://'.$savePath, FILE_EXISTS_REPLACE)) {
          // Create a file object.
          $file = new stdClass();
          $file->fid = NULL;
          $file->uri = $uri;
          $file->filename = drupal_basename($uri);
          $file->filemime = file_get_mimetype($file->uri);
          $file->uid      = $user->uid;
          $file->status   = 0;
          $existing_files = file_load_multiple(array(), array('uri' => $uri));
          if (count($existing_files)) {
            $existing = reset($existing_files);
            $file->fid = $existing->fid;
            $file->filename = $existing->filename;
          }

          $filedata = file_save($file);
        }

        return $filedata;
    }

    /**
     * 添加水印
     * @param $savePath
     * @return string
     */
    private function addWatermark($savePath) {
      $watermark_type = variable_get('ueditor_watermark_type', 'image');
      $watermark_place = variable_get('ueditor_watermark_place', 9);
      if($watermark_type == 'image'){
        $watermark_path = variable_get('ueditor_watermark_path', '');
        $watermark_alpha = variable_get('ueditor_watermark_alpha', 50);
        $success = $this->imageWaterMark($savePath, $watermark_place, $watermark_path, $watermark_alpha);
      }else{
        global $base_url;
        $watermark_textcontent = variable_get('ueditor_textcontent', $base_url);
        $watermark_textfontsize = variable_get('ueditor_textfontsize', 16);
        $watermark_textcolor = variable_get('ueditor_textcolor', '#FF0000');
        $success = $this->imageWaterMark($savePath, $watermark_place, '', '', $watermark_textcontent, $watermark_textfontsize, $watermark_textcolor);
      }

      return $success;
    }

    /**
     * 上传错误检查
     * @param $errCode
     * @return string
     */
    private function getStateInfo($errCode) {
        return !$this->stateMap[$errCode] ? $this->stateMap["ERROR_UNKNOWN"] : $this->stateMap[$errCode];
    }

    /**
     * 获取文件扩展名
     * @return string
     */
    private function getFileExt()
    {
        return strtolower(strrchr($this->oriName, '.'));
    }

    /**
     * 重命名文件
     * @return string
     */
    private function getFullName() {
        //替换日期事件
        $t = time();
        $d = explode('-', date("Y-y-m-d-H-i-s"));
        $format = $this->config["pathFormat"];
        $format = str_replace("{yyyy}", $d[0], $format);
        $format = str_replace("{yy}", $d[1], $format);
        $format = str_replace("{mm}", $d[2], $format);
        $format = str_replace("{dd}", $d[3], $format);
        $format = str_replace("{hh}", $d[4], $format);
        $format = str_replace("{ii}", $d[5], $format);
        $format = str_replace("{ss}", $d[6], $format);
        $format = str_replace("{time}", $t, $format);

        //过滤文件名的非法自负,并替换文件名
        $oriName = substr($this->oriName, 0, strrpos($this->oriName, '.'));
        $oriName = preg_replace("/[\|\?\"\<\>\/\*\\\\]+/", '', $oriName);
        $format = str_replace("{filename}", $oriName, $format);

        //替换随机字符串
        $randNum = rand(1, 10000000000) . rand(1, 10000000000);
        if (preg_match("/\{rand\:([\d]*)\}/i", $format, $matches)) {
            $format = preg_replace("/\{rand\:[\d]*\}/i", substr($randNum, 0, $matches[1]), $format);
        }

        //用Transliteration生成文件名
        if(strpos($format, '{transliteration_filename}')){
          if(module_exists('transliteration') && function_exists('transliteration_clean_filename')){
            $format = str_replace("{transliteration_filename}", transliteration_clean_filename($oriName), $format);
          }
        }       

        $ext = $this->getFileExt();
        return $format . $ext;
    }

    /**
     * 获取文件名
     * @return string
     */
    private function getFileName () {
        return substr($this->filePath, strrpos($this->filePath, '/') + 1);
    }

    /**
     * 获取文件完整路径
     * @return string
     */
    private function getFilePath() {
        $fullname = ueditor_get_savepath($this->fullName);
        $uploadPath = variable_get('file_public_path', conf_path() . '/files');
        $rootPath = strtr(DRUPAL_ROOT,'\\','/');

        if (substr($fullname, 0, 1) != '/') {
            $fullname = '/' . $fullname;
        }
 
        return $rootPath . '/' . $uploadPath . $fullname;
    }

    /**
     * 文件类型检测
     * @return bool
     */
    private function checkType() {
        return in_array($this->getFileExt(), $this->config["allowFiles"]);
    }

    /**
     * 文件大小检测
     * @return bool
     */
    private function  checkSize() {
        return $this->fileSize <= ($this->config["maxSize"]);
    }

    /**
     * 获取当前上传成功文件的各项信息
     * @return array
     */
    public function getFileInfo() {
        return array(
            "state" => $this->stateInfo,
            "url" => $this->fullName,
            "title" => $this->fileName,
            "original" => $this->oriName,
            "type" => $this->fileType,
            "size" => $this->fileSize
        );
    }

   /*
    * 功能：PHP图片水印 (水印支持图片或文字)
    * 参数：
    *$groundImage 背景图片，即需要加水印的图片，暂只支持GIF,JPG,PNG格式；
    *$waterPos水印位置，有10种状态，0为随机位置；
    *1为顶端居左，2为顶端居中，3为顶端居右；
    *4为中部居左，5为中部居中，6为中部居右；
    *7为底端居左，8为底端居中，9为底端居右；
    *$waterImage图片水印，即作为水印的图片，暂只支持GIF,JPG,PNG格式；
    *$waterText文字水印，即把文字作为为水印，支持ASCII码，不支持中文；
    *$textFont文字大小，值为1、2、3、4或5，默认为5；
    *$textColor文字颜色，值为十六进制颜色值，默认为#FF0000(红色)；
    *
    * 注意：Support GD 2.0，Support FreeType、GIF Read、GIF Create、JPG 、PNG
    *$waterImage 和 $waterText 最好不要同时使用，选其中之一即可，优先使用 $waterImage。
    *当$waterImage有效时，参数$waterString、$stringFont、$stringColor均不生效。
    *加水印后的图片的文件名和 $groundImage 一样。
    */
    private function imageWaterMark($groundImage, $waterPos = 0, $waterImage = '', $wateralpha = '', $waterText = '', $fontSize = '', $textColor = '') {
      $isWaterImage = FALSE;
      //读取水印文件
      if(!empty($waterImage) && file_exists($waterImage)) {
        $isWaterImage = TRUE;
        $water_info = getimagesize($waterImage);
        //取得水印图片的宽
        $water_w = $water_info[0];
        //取得水印图片的高
        $water_h = $water_info[1];
        //取得水印图片的格式
        switch($water_info[2]){
          case 1:
            $water_im = imagecreatefromgif($waterImage);
            break;
          case 2:
            $water_im = imagecreatefromjpeg($waterImage);
            break;
          case 3:
            $water_im = imagecreatefrompng($waterImage);
            break;
          default:
            return "ERROR_TYPE_NOT_ALLOWED";
        }
      }
      //读取背景图片
      if(!empty($groundImage) && file_exists($groundImage)) {
        $ground_info = getimagesize($groundImage);
        //取得背景图片的宽
        $ground_w = $ground_info[0];
        //取得背景图片的高
        $ground_h = $ground_info[1];
        //取得背景图片的格式
        switch($ground_info[2]){
          case 1:
            $ground_im = imagecreatefromgif($groundImage);
            break;
          case 2:
            $ground_im = imagecreatefromjpeg($groundImage);
            break;
          case 3:
            $ground_im = imagecreatefrompng($groundImage);
            break;
          default:
            return "ERROR_TYPE_NOT_ALLOWED";
        }
      }else{
        return "ERROR_WATERMARK_NOT_FOUND";
      }
      //水印位置
      //图片水印
      if($isWaterImage){
        $w = $water_w;
        $h = $water_h;
        $label = "图片的";
      }else{
        //文字水印
        $fontface = drupal_get_path('module', 'ueditor') . '/fonts/fzcyjt.ttf';
        $temp = imagettfbbox(ceil($fontSize*1.2),0,$fontface,$waterText);//取得使用 TrueType 字体的文本的范围
        $w = $temp[2] - $temp[6];
        $h = $temp[3] - $temp[7];
        unset($temp);
        $label = "文字区域";
      }
      if( ($ground_w<$w) || ($ground_h<$h) ){
        //echo "需要加水印的图片的长度或宽度比水印".$label."还小，无法生成水印！";
        return "ERROR_WATERMARK_SIZE";
      }
      switch($waterPos){
        case 0://随机
          $posX = rand(0,($ground_w - $w));
          $posY = rand(0,($ground_h - $h));
          break;
        case 1://1为顶端居左
          $posX = 10;
          $posY = $h + 10;
          break;
        case 2://2为顶端居中
          $posX = ($ground_w - $w) / 2;
          $posY = $h + 10;
          break;
        case 3://3为顶端居右
          $posX = $ground_w - $w;
          $posY = $h + 10;
          break;
        case 4://4为中部居左
          $posX = 10;
          $posY = ($ground_h - $h) / 2;
          break;
        case 5://5为中部居中
          $posX = ($ground_w - $w) / 2;
          $posY = ($ground_h - $h) / 2;
          break;
        case 6://6为中部居右
          $posX = $ground_w - $w;
          $posY = ($ground_h - $h) / 2;
          break;
        case 7://7为底端居左
          $posX = 10;
          $posY = $ground_h - $h;
          break;
        case 8://8为底端居中
          $posX = ($ground_w - $w) / 2;
          $posY = $ground_h - $h;
          break;
        case 9://9为底端居右
          $posX = $ground_w - $w - 10;   // -10 是距离右侧10px 可以自己调节
          $posY = $ground_h - $h - 10;   // -10 是距离底部10px 可以自己调节
          break;
        default://随机
          $posX = rand(0,($ground_w - $w));
          $posY = rand(0,($ground_h - $h));
          break;
      }
      //设定图像的混色模式
      imagealphablending($ground_im, true);
      //图片水印
      if($isWaterImage){        
        imagecopymerge($ground_im, $water_im, $posX, $posY, 0, 0, $water_w, $water_h, $wateralpha);//拷贝水印到目标文件 
      }else{
        //文字水印
        if(!empty($textColor) && (strlen($textColor)==7)){
          $R = hexdec(substr($textColor,1,2));
          $G = hexdec(substr($textColor,3,2));
          $B = hexdec(substr($textColor,5));
        }else{
          return "ERROR_WATERMARK_TEXT_RGB";
        }
        imagettftext($ground_im,$fontSize,0,$posX,$posY,imagecolorallocate($ground_im,$R,$G,$B),$fontface,$waterText);
      }
      //生成水印后的图片
      @unlink($groundImage);
      //取得背景图片的格式
      switch($ground_info[2]){
        case 1:
          imagegif($ground_im,$groundImage);
          break;
        case 2:
          imagejpeg($ground_im,$groundImage);
          break;
        case 3:
          imagepng($ground_im,$groundImage);
          break;
        default:
          return "ERROR_WATERMARK_ADD";
      }

      //释放内存
      if(isset($water_info)){
        unset($water_info);
      }
      if(isset($water_im)){
        imagedestroy($water_im);
      }
      unset($ground_info);
      imagedestroy($ground_im);
      return TRUE;
    }
}
