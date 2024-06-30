import bcrypt
import pyotp
import smtplib
import requests
ending = "\n\n\n\nClick - Password Manager and File Encryption Service\nSee our docs at https://github.com/PhiJam1/Click"




def GetLocationInfo():
    response = requests.get("http://ipinfo.io/json")
    loc_info = dict(ip = "UNKNOWN", city = "UNKNOWN", region = "UNKNOWN", country = "UNKNOWN")
    if response.status_code == 200:
        data = response.json()
        loc_info["ip"] = data.get("ip")
        loc_info["city"] = data.get("city")
        loc_info["region"] = data.get("region")
        loc_info["country"] = data.get("country")
        # loc = data.get("loc")  # this is latitude,longitude
    return loc_info

def Register_Password(password):
    bytes = password.encode('utf-8')
    salt = bcrypt.gensalt()
    hash = bcrypt.hashpw(bytes, salt)
    hash = str(hash)
    # Strip off the PyByte notation
    return hash[2:len(hash) - 1]

def Verify_Password(given_password, known_hash):
    # The encode() function turns these strings back to bytes
    if (bcrypt.checkpw(given_password.encode('utf-8'), known_hash.encode('utf-8'))):
        return 1.0
    else:
        return 2.0

def GenerateOTP(user_email, sys_email, sys_key, time_interval):
    key = pyotp.random_base32()
    totp = pyotp.TOTP(key, interval=time_interval)
    code = totp.now()
    subject = "One Time Password"
    msg = "This is your code: " + code + "\nIt will expire within 10 minutes. Please don't share this with anyone."
    msg += ending
    text = f"Subject: {subject}\n\n{msg}"
    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.starttls()
    server.login(sys_email, sys_key)
    server.sendmail(sys_email, user_email, text)
    server.quit()
    return(str(key))

def CheckOTP(code, key, time_interval):
    totp = pyotp.TOTP(key, interval=time_interval)
    if (totp.verify(code)):
        return 1.0
    else:
        return 2.0

# we could do something with getting the machine's location
# We should send the mac address at least
# maybe even try to ban users by adding their mac address to
# a useer's banned list
def SendWarningEmail(user_email, sys_email, sys_key, mac_address):
    loc_info = GetLocationInfo()
    subject = "Warning: Excessive Login Attempts"
    msg = "Someone tried to access your account and failed several times. They have your email and password.\n"
    msg += " This person's location information is provided below\n"
    msg += "Country: " + loc_info['country'] + "\nRegion: " + loc_info["region"] + "\n"
    msg += "City: " + loc_info['city'] + "\nIP Address: " + loc_info["ip"] + "\n"
    msg += "Mac Address: " + mac_address
    msg += "\nIf you don't recognize this activity, please log into your account and banned this mac address from further attempts."
    msg += ending

    text = f"Subject: {subject}\n\n{msg}"
    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.starttls()
    server.login(sys_email, sys_key)
    server.sendmail(sys_email, user_email, text)
    server.quit()


def NewDeviceDetected(user_email, sys_email, sys_key, mac_address):
    loc_info = GetLocationInfo()
    subject = "New Device Login"
    msg = "Someone has tried to login to your account. "
    msg += "This person's location information is provided below\n"
    msg += "Country: " + loc_info['country'] + "\nRegion: " + loc_info["region"] + "\n"
    msg += "City: " + loc_info['city'] + "\nIP Address: " + loc_info["ip"] + "\n"
    msg += "Mac Address: " + mac_address
    msg += "\n If you don't recognize this activity, please log into your account and banned this mac address from further attempts."
    msg += ending
    text = f"Subject: {subject}\n\n{msg}"
    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.starttls()
    server.login(sys_email, sys_key)
    server.sendmail(sys_email, user_email, text)
    server.quit()


def BannedDeviceLoginAttempt(user_email, sys_email, sys_key, mac_address):
    loc_info = GetLocationInfo()
    subject = "Warning: Banned Account Login Attempt"
    msg = "Someone with a mac address you banned has tried to access your account.\n"
    msg += "This person's location information is provided below\n"
    msg += "Country: " + loc_info['country'] + "\n Region: " + loc_info["region"] + "\n"
    msg += "City: " + loc_info['city'] + "\n IP Address: " + loc_info["ip"] + "\n"
    msg += "Mac Address: " + mac_address
    msg += ending
    text = f"Subject: {subject}\n\n{msg}"
    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.starttls()
    server.login(sys_email, sys_key)
    server.sendmail(sys_email, user_email, text)
    server.quit()
