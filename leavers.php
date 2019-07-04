<?php
/*************************************************************************
remove visitor from database when they leave the page.
==========================================================================
adapted from www.thomas.vanhoutte.be/miniblog/visitor-counter/
*************************************************************************/
ignore_user_abort(true);

$dbfile = "visitors.db"; // path to file storing number of visitors
$ipfile = "queue.db"; // path to file storing order of visitors
$adfile = "audience.db"; // path to file for storing ips of audience members currently viewing the installation

if(!file_exists($dbfile)) {
    die("Error: Data file " . $dbfile . " NOT FOUND!");
}
if(!is_writable($dbfile)) {
    die("Error: Data file " . $dbfile . " is NOT writable! Please CHMOD it to 666!");
}
if(!file_exists($ipfile)) {
    die("Error: Data file " . $ipfile . " NOT FOUND!");
}
if(!is_writable($ipfile)) {
    die("Error: Data file " . $ipfile . " is NOT writable! Please CHMOD it to 666!");
}
if(!file_exists($ipfile)) {
    die("Error: Data file " . $adfile . " NOT FOUND!");
}
if(!is_writable($ipfile)) {
    die("Error: Data file " . $adfile . " is NOT writable! Please CHMOD it to 666!");
}

 
function RemoveVisitors() {
    global $dbfile;
    $cur_ip = getIP();
    $dbary_new = array();
    
    // unpack array and re-add everyone who is not the current user
    $dbary = unserialize(file_get_contents($dbfile));
    if(is_array($dbary)) {
        while(list($user_ip, $user_time) = each($dbary)) {
            if($user_ip != $cur_ip) { // check whether the ip is the same
                $dbary_new[$user_ip] = $user_time; // adding them into the new array if not the same
            }
        }
    }
    // the list item with the current ip is removed, changing the number.
    $fp = fopen($dbfile, "w");
    fputs($fp, serialize($dbary_new));
    fclose($fp);
    $out = sprintf("%03d", count($dbary_new)); // format the result to display 3 digits with leading 0's
    return $out;
}

function RemoveFromQueue() {
    global $ipfile;
    $cur_ip = getIP();
    $dbary_new_queue = array();
 
    // unpack array and re-add everyone who is not the current user
    $dbary = unserialize(file_get_contents($ipfile));
    if(is_array($dbary)) {
        while(list($user_ip, $user_time) = each($dbary)) {
            if($user_ip != $cur_ip) { // check whether the ip is the same
                $dbary_new_queue[$user_ip] = $user_time; // adding them into the new array if not the same
            }
        }
    }

    // sort the new array in ascending order by value (entrance time)
    asort($dbary_new_queue); 
    $fp = fopen($ipfile, "w");
    fputs($fp, serialize($dbary_new_queue));
    fclose($fp);
}

function RemoveFromAudience() {
    global $adfile;
    $cur_ip = getIP();
    $dbary_new = array();
 
    // unpack array and re-add everyone who is not the current user
    $dbary = unserialize(file_get_contents($adfile));
    if(is_array($dbary)) {
        while(list($user_ip, $user_time) = each($dbary)) {
            if($user_ip != $cur_ip) { // check whether the ip is the same
                $dbary_new[$user_ip] = $user_time; // adding them into the new array if not the same
            }
        }
    }

    $fp = fopen($adfile, "w");
    fputs($fp, serialize($dbary_new));
    fclose($fp);
 
 // this will become visitornum
    $out = sprintf("%03d", count($dbary_new)); // format the result to display 3 digits with leading 0's
    return $out;
}

function getIP() {
    if(isset($_SERVER['HTTP_X_FORWARDED_FOR'])) $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    elseif(isset($_SERVER['REMOTE_ADDR'])) $ip = $_SERVER['REMOTE_ADDR'];
    else $ip = "0";
    return $ip;
}

RemoveVisitors();
RemoveFromQueue();
$audiencenum = RemoveFromAudience();

$fp = fopen('visitornum.txt', 'w');
fwrite($fp, $audiencenum);
fclose($fp);
?>
