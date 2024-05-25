import bcrypt

def Register_Password(password):
    bytes = password.encode('utf-8')
    salt = bcrypt.gensalt()
    hash = bcrypt.hashpw(bytes, salt)
    hash = str(hash)
    # Strip off the PyByte notation
    return hash[2:len(hash) - 1]

def Verify_Password(known_hash, given_password):
    # The encode() function turns these strings back to bytes
    return bcrypt.checkpw(given_password.encode('utf-8'), known_hash.encode('utf-8'))
