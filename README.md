# mTron

In this demo the user is tasked by the MCP to test the process tracking system on the ENCOM Grid.
The user checks messages from the MCP for task instructions, tracks a process by entering Tron's
current sector position and memorising the output, then sending a message to the MCP with Tron's new
sector location (or sending 'reset' to reset the test).

This implementation uses custom action and service interfaces defined in the grid_interfaces package.

# What's going on
The user only ever interacts with the User node which manages I/O. On startup, the MCP node
publishes a message to the Grid node (via **topic**) containing the position of Tron. The user can
request this message (menu option 1) from the Grid using a **service**.

The user can begin the test
(menu option 3) and enters the position provided by the MCP. This initiates an **action** on Grid
that sends the new position over one digit at a time via the action **feedback**, then the whole position
is sent at the end as a **result**.

The user sends this new position to the MCP (menu option 2)
by publishing it to a **topic**. The MCP checks with the Grid if the user sent the correct position
(via a **service**) and sends the user a congratulatory or failure message.

# Launching
In the parent folder 'mtron' run `colcon build`, source the overlay using `source install/setup.bash`,
then launch the three nodes in separate terminals using `ros2 launch grid grid`,
`ros2 launch mcp mcp`, and `ros2 launch user user`. User interaction occurs only in the terminal
running the user node.
