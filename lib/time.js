module.exports = {
  now: () => new Date().toLocaleString(),
  year: () => new Date().getFullYear(),
  month: () => new Date().getMonth() + 1,
  day: () => new Date().getDate()
};
