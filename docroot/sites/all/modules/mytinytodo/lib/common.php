<?php

/**
 * Check view and edit permissions.
 *
 * @param $op
 *   The type of operation. Either 'view' or 'edit'.
 */
function have_access($op) {
    global $user;

    $db = DBConnection::instance();
    $field_id = (int)_post('fid');
    if (!$field_id)
        $field_id = (int)_get('fid');

    $field = (object)$db->dq("SELECT entity_id, entity_type, delta FROM {mytinytodo_fields} WHERE id = ?", $field_id)->fetch_assoc();
    $field_info = field_info_field_by_id($field->delta);

    if ($field->entity_type == 'node') {
        if (!($node = node_load($field->entity_id))) {
            return false;
        }

        $node_access = $op == 'edit' ? 'update' : $op;
        if (node_access($node_access, $node, $user) && field_access($op, $field_info, $field->entity_type, $node, $user)) {
            return true;
        }
    } else if ($field->entity_type == 'user') {
        if (!($account = user_load($field->entity_id))) {
            return false;
        }

        if (field_access($op, $field_info, $field->entity_type, $account, $user)) {
            return true;
        }
    } else if ($field->entity_type == 'comment') {
        if (!($comment = comment_load($field->entity_id))) {
            return false;
        }

        if ($op == 'view' && !user_access('access comments')) {
            return false;
        } else if ($op == 'edit' && !comment_access($op, $comment)) {
            return false;
        }

        if (field_access($op, $field_info, $field->entity_type, $comment, $user)) {
            return true;
        }
    } else if (module_exists('entity')) {
        if (!($entity = entity_load($field_id))) {
            return false;
        }

        $entity_access = $op == 'edit' ? 'update' : $op;
        if (entity_access($entity_access, $field->entity_type, $entity, $user) && field_access($op, $field_info, $field->entity_type, $entity, $user)) {
            return true;
        }
    }

    return false;
}

function have_read_access($listId = null) {
    return have_access('view');
}

function check_read_access($listId = null) {
    if (have_read_access($listId))
        return;

    jsonExit( array('total' => 0, 'list' => array(), 'denied' => 1) );
}

function check_write_access($listId = null) {
    if (have_write_access($listId))
        return;

    jsonExit( array('total' => 0, 'list' => array(), 'denied' => 1) );
}

function have_write_access($listId = null) {
    return valid_token() && have_access('edit');
}

function valid_token() {
    $token = isset($_POST['csrf']) ? $_POST['csrf'] : '';
    return drupal_valid_token($token, 'mytinytodo');
}

function htmlarray($a, $exclude = null) {
    htmlarray_ref($a, $exclude);
    return $a;
}

function htmlarray_ref(&$a, $exclude=null) {
    if (!$a)
        return;

    if (!is_array($a)) {
        $a = htmlspecialchars($a);
        return;
    }

    reset($a);

    if ($exclude && !is_array($exclude))
        $exclude = array($exclude);

    foreach($a as $k => $v) {
        if (is_array($v))
            $a[$k] = htmlarray($v, $exclude);
        else if (!$exclude)
            $a[$k] = htmlspecialchars($v);
        else if (!in_array($k, $exclude))
            $a[$k] = htmlspecialchars($v);
    }
}

function stop_gpc(&$arr) {
    if (!is_array($arr))
        return 1;

    if (!get_magic_quotes_gpc())
        return 1;

    reset($arr);

    foreach($arr as $k => $v) {
        if (is_array($arr[$k]))
            stop_gpc($arr[$k]);
        else if (is_string($arr[$k]))
            $arr[$k] = stripslashes($v);
    }

    return 1;
}

function _post($param,$defvalue = '') {
    if (!isset($_POST[$param])) {
        return $defvalue;
    }
    else {
        return $_POST[$param];
    }
}

function _get($param,$defvalue = '') {
    if (!isset($_GET[$param])) {
        return $defvalue;
    }
    else {
        return $_GET[$param];
    }
} 

class Config {
    public static $params = array(
        'db' => array('default' => 'sqlite', 'type' => 's'),
        'mysql.host' => array('default' => 'localhost', 'type' => 's'),
        'mysql.db' => array('default' => 'mytinytodo', 'type' => 's'),
        'mysql.user' => array('default' => 'user', 'type' => 's'),
        'mysql.password' => array('default' => '', 'type' => 's'),
        'prefix' => array('default' => '', 'type' => 's'),
        'url' => array('default' => '', 'type' => 's'),
        'mtt_url' => array('default' => '', 'type' => 's'),
        'title' => array('default' => '', 'type' => 's'),
        'lang' => array('default' => 'en', 'type' => 's'),
        'password' => array('default' => '', 'type' => 's'),
        'smartsyntax' => array('default' => 1, 'type' => 'i'),
        'timezone' => array('default' => 'UTC', 'type' => 's'),
        'autotag' => array('default' => 1, 'type' => 'i'),
        'duedateformat' => array('default' => 1, 'type' => 'i'),
        'firstdayofweek' => array('default' => 1, 'type' => 'i'),
        'session' => array('default' => 'files', 'type' => 's', 'options' => array('files', 'default')),
        'clock' => array('default' => 24, 'type' => 'i', 'options' => array(12, 24)),
        'dateformat' => array('default' => 'j M Y', 'type' => 's'),
        'dateformat2' => array('default' => 'n/j/y', 'type' => 's'),
        'dateformatshort' => array('default' => 'j M', 'type' => 's'),
        'template' => array('default' => 'default', 'type' => 's'),
        'showdate' => array('default' => 0, 'type' => 'i'),
    );

    public static $config;

    public static function loadConfig() {
        if (!isset($_GET['fid']))
            return;

        $db = DBConnection::init(new Database_Drupal);
        self::$config = unserialize($db->sq("SELECT settings FROM {mytinytodo_lists} WHERE field_id = ?", array((int)$_GET['fid'])));
    }

    public static function get($key) {
        if (isset(self::$config[$key]))
            return self::$config[$key];
        else if (isset(self::$params[$key]))
            return self::$params[$key]['default'];
        else
            return null;
    }

    public static function set($key, $value) {
        self::$config[$key] = $value;
    }

    public static function save() {
        $db = DBConnection::init(new Database_Drupal);
        db_update('mytinytodo_lists')
            ->fields(
                array(
                    'settings' => serialize(self::$config),
                )
            )
            ->condition('field_id', self::$config['field_id'])
            ->execute();
    }    
}

function formatDate3($format, $ay, $am, $ad, $lang) {
    # F - month long, M - month short
    # m - month 2-digit, n - month 1-digit
    # d - day 2-digit, j - day 1-digit
    $ml = $lang->get('months_long');
    $ms = $lang->get('months_short');
    $Y = $ay;
    $y = $Y < 2010 ? '0'.($Y-2000) : $Y-2000;
    $n = $am;
    $m = $n < 10 ? '0'.$n : $n;
    $F = $ml[$am-1];
    $M = $ms[$am-1];
    $j = $ad;
    $d = $j < 10 ? '0'.$j : $j;

    return strtr($format, array('Y' => $Y, 'y' => $y, 'F' => $F, 'M' => $M, 'n' => $n, 'm' => $m, 'd' => $d, 'j' => $j));
}

function url_dir($url) {
    if (false !== $p = strpos($url, '?')) {
        $url = substr($url,0,$p); # to avoid parse errors on strange query strings
    }

    $p = parse_url($url, PHP_URL_PATH);

    if ($p == '')
        return '/';
    if (substr($p,-1) == '/')
        return $p;
    if (false !== $pos = strrpos($p,'/'))
        return substr($p,0,$pos+1);

    return '/';
}

function escapeTags($s) {
    $c1 = chr(1);
    $c2 = chr(2);
    $s = preg_replace("~<b>([\s\S]*?)</b>~i", "${c1}b${c2}\$1${c1}/b${c2}", $s);
    $s = preg_replace("~<i>([\s\S]*?)</i>~i", "${c1}i${c2}\$1${c1}/i${c2}", $s);	
    $s = preg_replace("~<u>([\s\S]*?)</u>~i", "${c1}u${c2}\$1${c1}/u${c2}", $s);
    $s = preg_replace("~<s>([\s\S]*?)</s>~i", "${c1}s${c2}\$1${c1}/s${c2}", $s);
    $s = str_replace(array($c1, $c2), array('<','>'), htmlspecialchars($s));

    return $s;
}

/* Found in comments on http://www.php.net/manual/en/function.uniqid.php#94959 */
function generateUUID() {
    return sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
        mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff),
        mt_rand(0, 0x0fff) | 0x4000,
        mt_rand(0, 0x3fff) | 0x8000,
        mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff)
    );
}

class DBConnection {
    protected static $instance;

    public static function init($instance) {
        self::$instance = $instance;
        return $instance;
    }

    public static function instance() {
        if (!isset(self::$instance)) {
            //$c = __CLASS__;
            $c = 'DBConnection';
            self::$instance = new $c;
        }

        return self::$instance;	
    }
}
