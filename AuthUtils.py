import bcrypt
import time
import pyotp
import smtplib

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
    subject = "One Time Password Code"
    msg = "Here's your code: " + code
    text = f"Subject: {subject}\n\n{msg}"
    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.starttls()
    server.login(sys_email, sys_key)
    server.sendmail(sys_email, user_email, text)
    return(str(key))

def CheckOTP(code, key, time_interval):
    totp = pyotp.TOTP(key, interval=time_interval)
    if (totp.verify(code)):
        return 1.0
    else:
        return 2.0
