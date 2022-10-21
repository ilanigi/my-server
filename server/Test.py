from Server import Server


def Test(server:Server):
        print("Test User exist")
        print("     When user is not exist")
        exist = server.services.users.user_exist("gilad")
        if exist:
            raise Exception('user should not exist')
        else:
            print("         user not exist")
        
        server.services.users.save_new_to_db("gilad")
        print("     When user exist")
        exist = server.services.users.user_exist("gilad")
        if not exist:
            raise Exception('user should exist')
        else:
            print("user exist")

