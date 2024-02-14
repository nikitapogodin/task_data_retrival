Task: Client-Server Data Retrieval System

Description:
Your task is to design and implement a client-server system for data retrieval. The server will manage multiple data sources, each generating data with a specific lifetime. The goal is to ensure that the client can request data from any source, and the server should respond with the most up-to-date information.

Requirements:

    Server Configuration:
        The server should be configurable with the number of data sources it will manage.

    Data Generation:
        Each data source should be capable of updating data periodicly.

    Client-Server Communication:
        Design a communication protocol between the client and server for data retrieval requests.
        The client should be able to send a request specifying the data source it wants to retrieve data from.

    Handling Delay:
    	Assume retrieving data from a data source takes considerable time. Implement a mechanism on the server side to avoid delays caused by data source retrieval.
        Ensure that the client receives a timely response without being affected by the data retriving delay.

    Response Handling:
        The server should respond with a success message and the requested data if available and within datas lifetime.
        If the data is no longer available, the server should respond with an appropriate error message indicating that the data is not available.

    Data Freshness:
        The server should not respond with outdated data. Ensure that the client always receives the latest available data.

Submission:

    Provide the source code for both the client and server components, by extending and changing provided skeleton. Use git and provide history of commits
    Include any necessary documentation on how to configure, run, and test the system.
    Docker configuration to build and run docker and server.

Note:

    Use C++ and Python programing languages.
    DataRetServer skeleton, server implementation and python client are provided as reference; feel free to use as is or change (not breaking base asumptions).
    Assume Ubuntu 22.04 as the base system.
    Feel free to ask for clarification if needed.
