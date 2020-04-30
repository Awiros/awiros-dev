#ifndef _RESOURCE_MANAGE_H_
#define _RESOURCE_MANAGE_H_

#include <iostream>
#include <memory>
#include <map>
#include <mutex>
#include <condition_variable>

/*
 * Register resources here, maintain shared count and release accordingly
 * Register resources by name and store in a hashtable
 * */

class awiros_resource{

    /* Shared counter */

    /* Locking */
    std::mutex m;
    std::condition_variable cv;

    /* Access level*/

    /* underlying object */

    public:
        int shared_counter = 1;

        awiros_resource();

        /* Destructor (Introduce run-time polymorphism) */
        virtual ~awiros_resource();

        /* resource size (implicit) */
};

class awiros_resource_registry{

    /* In System Initialization */

    /* Global symbol registry */
    std::map<std::string, awiros_resource*> m_registry;

    /* Locking */
    std::mutex m;
    std::condition_variable cv;

	public:
        awiros_resource_registry();

        /* log global registry */
        void print_info();

        /* register */
        template<typename T>
        void register_resource(const std::string& resource_name, T* resource){

            /* locking */

            /* Check presence */
            auto resource_in = this->m_registry.find(resource_name);
            if(resource_in !=  this->m_registry.end()){
                std::cout << "Already present: Use different key" << std::endl;
                return;
            }

            std::cout << "Register resource" << std::endl;

            this->m_registry[resource_name] = resource;

            return;
        } ;

	/* update resource */
	template<typename T>
	void update_resource(const std::string& resource_name, T* resource){
		this->m_registry[resource_name] = resource;
	}

        /* de-register */
        void deregister_resource(const std::string&);

	/* Delete */
	void delete_resource(const std::string&);

        /* fetch resource */
        template<typename T>
        T* get_resource(const std::string& resource_name){

            /* locking*/

            /* fetch resource */
            auto resource = this->m_registry.find(resource_name);

            /* Check presence */
            if(resource == this->m_registry.end()){
                return (T*)NULL;
            }

            /* share increased*/
            resource->second->shared_counter++;

            /* Dynamic downcast */
            T* resource_t = dynamic_cast<T*>(this->m_registry[resource_name]);

            return resource_t;

        };

	/* Fetching camera resource */
	template<typename T>
	T* get_camera_resource(const std::string& resource_name){
			
		// locking

		// fetch resource 
		auto resource = this->m_registry.find(resource_name);

		// Check presence 
		if(resource == this->m_registry.end()){
			return (T*)NULL;
		}

		// Dynamic downcast 
		T* resource_t = dynamic_cast<T*>(this->m_registry[resource_name]);

		return resource_t;

	};

	int get_counter(std::string res_name);

};

extern awiros_resource_registry g_resource_registry;

#endif// _RESOURCE_MANAGE_H_
