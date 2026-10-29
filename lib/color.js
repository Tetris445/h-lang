module.exports = {
  red: (text) => '\x1b[31m' + text + '\x1b[0m',
  green: (text) => '\x1b[32m' + text + '\x1b[0m',
  yellow: (text) => '\x1b[33m' + text + '\x1b[0m',
  blue: (text) => '\x1b[34m' + text + '\x1b[0m'
};
