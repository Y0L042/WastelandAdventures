-- Set a unique leader key mapping to display a message
vim.api.nvim_set_keymap('n', '<leader>t', ':echo "Local Config Loaded!"<CR>', { noremap = true, silent = true, desc = "Test Local Config" })

