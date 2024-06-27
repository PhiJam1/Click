import numpy as np
from numpy.polynomial.polynomial import Polynomial

import random
n = 256 # max degree of any polynomial
k = 2   # number of polynomials in a vector
POLY_MODULO = []
for i in range(n):
  POLY_MODULO.append(0)
POLY_MODULO[0] = 1
POLY_MODULO[-1] = 1 # represents x^n + 1

COEFF_MODULO = 3329


# Some utility functions to help with mathematical operations

def add_poly(p1, p2):
    res = [0] * max(len(p1), len(p2))
    for i in range(len(res)):
        if (i < len(p1)):
            res[i] += p1[i]
        if (i < len(p2)):
            res[i] += p2[i]
        # make sure coefficients stay under modulo q
        res[i] %= COEFF_MODULO
    return res

# This function will return the inverse of the given polynomial
# It will preform if needed, which is you may get large numbers if you
# were expecting a negative number.
def inv_poly(poly):
  return list(map(lambda x: -x % COEFF_MODULO, poly))


def sub_poly(p1, p2):
    return add_poly(p1, inv_poly(p2))




def mul_poly_simple(p1, p2):
  tmp = [0] * (max(len(p1), len(p2)) * 2 - 1) # the product of two degree n polynomial cannot exceed 2n

  # schoolbook multiplication
  for i in range(len(p1)):
    # perform a_i * b
    for j in range(len(p2)):
      tmp[i + j] += p1[i] * p2[j]

  # take polynomial modulo f
  # since Kyber's poly modulus is always x^n + 1,
  # we can efficiently compute the remainder
  degree_f = len(POLY_MODULO) - 1
  for i in range(degree_f, len(tmp)):
    tmp[i - degree_f] -= tmp[i]
    tmp[i] = 0

  # take coefficients modulo q
  tmp = list(map(lambda x: x % COEFF_MODULO, tmp))
  return tmp[:degree_f]



def add_vec(v0, v1):
  if (len(v0) != len(v1)):
    return []

  res = []

  for i in range(len(v0)):
    res.append(add_poly(v0[i], v1[i]))
  return res

def mul_vec_simple(v0, v1):
  if (len(v0) != len(v1)): # sizes need to be the same
    return []
  degree_f = len(POLY_MODULO) - 1
  result = [0 for i in range(degree_f - 1)]

  # textbook vector inner product
  for i in range(len(v0)):
    result = add_poly(result, mul_poly_simple(v0[i], v1[i]))

  return result


def mul_mat_vec_simple(m, a):
  result = []

  # textbook matrix-vector multiplication
  for i in range(len(m)):
    result.append(mul_vec_simple(m[i], a))

  return result


def transpose(m):
  result = [[None for i in range(len(m))] for j in range(len(m[0]))]

  for i in range(len(m)):
    for j in range(len(m[0])):
      result[j][i] = m[i][j]

  return result


def encrypt(A, t, plaintext, r, e1, e2):
  half_q = int(COEFF_MODULO / 2 + 0.5)
  m = list(map(lambda x : x * half_q, plaintext))

  u = add_vec(mul_mat_vec_simple(transpose(A), r), e1)
  v = sub_poly(add_poly(mul_vec_simple(t, r), e2), m)
  return u, v

def decrypt(s, u, v):
  m_n = sub_poly(v, mul_vec_simple(s, u))

  half_q = int(COEFF_MODULO / 2 + 0.5)
  def round(val, center, bound):
    dist_center = np.abs(center - val)
    dist_bound = min(val, bound - val)
    return center if dist_center < dist_bound else 0

  m_n = list(map(lambda x: round(x, half_q, COEFF_MODULO), m_n))
  m_b = list(map(lambda x: x // half_q, m_n))

  return m_b





# https://cryptographycaffe.sandboxaq.com/posts/kyber-01/

def binary_to_string(bits):
    return ''.join([chr(int(i, 2)) for i in bits])

def convert_msg_to_bin(msg):
  # Convert to a full bin string
  msg_bin = ''.join(format(i, '08b') for i in bytearray(msg, encoding ='utf-8'))
  secret = []

  # convert to a list
  for i in range(len(msg_bin)):
    secret.append(int(msg_bin[i]))
  return secret

def convert_bin_to_msg(bin):
  letters = []
  buff = ""
  msg = ""
  # Loop through in chunks of 8
  for i in range(len(bin)):
    buff += str(bin[i])
    if ((i + 1) % 8 == 0):
      msg += chr(int(buff, 2))
      buff = ""
  return msg


def GenKey(password):
  password_bin = convert_msg_to_bin(password)
  s = [password_bin[0:int(len(password_bin) / 2)], password_bin[int(len(password_bin) / 2): ]]
  A = (np.random.random([k, k, n - 1]) * COEFF_MODULO).astype(int)  # can be computed at runtime and forgotten
  e = (np.random.random([k, n - 1]) * 3).astype(int) - 1            # can be randomly computed at runtime and safely forgotten
  t = add_vec(mul_mat_vec_simple(A, s), e)                          # Product of other terms
  return (A, t, s)

# This function should be able to take in some plaintext and a password and output the ciphertext
def EncryptionDriver(plaintext, password):
  # compute bin string of plaintext
  plaintext_bin = convert_msg_to_bin(plaintext)

  # Get the public and private key
  A, t, s = GenKey(password)

  # Use the public key to encrypt the plaintext
  # Encryption params
  r = (np.random.random([k, n - 1]) * 3).astype(int) - 1
  e_1 = (np.random.random([k, n - 1]) * 3).astype(int) - 1
  e_2 = (np.random.random([n - 1]) * 3).astype(int) - 1
  u, v = encrypt(A, t, plaintext_bin, r, e_1, e_2)

  # return u, v - the ciphertext, and s - the private key
  return u, v, s

def DecryptionDriver(password, u, v):
  b1, b2, s = GenKey(password)
  plaintext_bin = decrypt(s, u, v)
  return convert_bin_to_msg(plaintext_bin)

